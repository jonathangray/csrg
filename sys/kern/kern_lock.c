/* 
 * Copyright (c) 1995
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Avadis Tevanian, Jr., Michael Wayne Young, and the Mach Operating
 * System project at Carnegie-Mellon University.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)kern_lock.c	8.1 (Berkeley) 04/09/95
 */

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/lock.h>

/*
 * Locking primitives implementation.
 * Locks provide shared/exclusive sychronization.
 */

#if NCPUS > 1

/*
 * For multiprocessor system, try spin lock first.
 *
 * This should be inline expanded below, but we cannot have #if
 * inside a multiline define.
 */
int lock_wait_time = 100;
#define PAUSE(lkp, wanted)						\
		if (lock_wait_time > 0) {				\
			int i;						\
									\
			atomic_unlock(&lkp->lk_interlock);		\
			for (i = lock_wait_time; i > 0; i--)		\
				if (!(wanted))				\
					break;				\
			atomic_lock(&lkp->lk_interlock);		\
		}							\
		if (!(wanted))						\
			break;

#else /* NCPUS == 1 */

/*
 * It is an error to spin on a uniprocessor as nothing will ever cause
 * the atomic lock to clear while we are executing.
 */
#define PAUSE(lkp, wanted)

#endif /* NCPUS == 1 */

/*
 * Acquire a resource.
 */
#define ACQUIRE(lkp, error, extflags, wanted)				\
	PAUSE(lkp, wanted);						\
	for (error = 0; wanted; ) {					\
		(lkp)->lk_flags |= LK_WAITING;				\
		atomic_unlock(&(lkp)->lk_interlock);			\
		error = tsleep(lkp, (lkp)->lk_prio, (lkp)->lk_wmesg,	\
		    (lkp)->lk_timo);					\
		atomic_lock(&(lkp)->lk_interlock);			\
		(lkp)->lk_flags |= LK_SLEPT;				\
		if (error)						\
			break;						\
		if ((extflags) & LK_SLEEPFAIL) {			\
			error = ENOLCK;					\
			break;						\
		}							\
	}

/*
 * Initialize a lock; required before use.
 */
void lock_init(lkp, prio, wmesg, timo, flags)
	struct lock *lkp;
	int prio;
	char *wmesg;
	int timo;
	int flags;
{
	bzero(lkp, sizeof(struct lock));
	atomic_lock_init(&lkp->lk_interlock);
	lkp->lk_flags = flags & LK_EXTFLG_MASK;
	lkp->lk_prio = prio;
	lkp->lk_timo = timo;
	lkp->lk_wmesg = wmesg;
	lkp->lk_lockholder = LK_NOPROC;
}

/*
 * Set, change, or release a lock.
 *
 * Shared requests increment the shared count. Exclusive requests set the
 * LK_WANT_EXCL flag (preventing further shared locks), and wait for already
 * accepted shared locks and shared-to-exclusive upgrades to go away.
 */
lockmgr(lkp, p, flags)
	struct lock *lkp;
	struct proc *p;
	int flags;
{
	pid_t pid;
	int error, extflags;

	pid = p->p_pid;
	extflags = (flags | lkp->lk_flags) & LK_EXTFLG_MASK;
	lkp->lk_flags &= ~LK_SLEPT;

	switch (flags & LK_TYPE_MASK) {

	case LK_SHARED:
		atomic_lock(&lkp->lk_interlock);
		if (lkp->lk_lockholder != pid) {
			/*
			 * If just polling, check to see if we will block.
			 */
			if ((extflags & LK_NOWAIT) && (lkp->lk_flags &
			    (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE))) {
				atomic_unlock(&lkp->lk_interlock);
				return (EBUSY);
			}
			/*
			 * Wait for exclusive locks and upgrades to clear.
			 */
			ACQUIRE(lkp, error, extflags, lkp->lk_flags &
			    (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE));
			if (error) {
				atomic_unlock(&lkp->lk_interlock);
				return (error);
			}
			lkp->lk_sharecount++;
			atomic_unlock(&lkp->lk_interlock);
			return (0);
		}
		/*
		 * We hold an exclusive lock, so downgrade it to shared.
		 * An alternative would be to fail with EDEADLK.
		 */
		lkp->lk_sharecount++;
		atomic_unlock(&lkp->lk_interlock);
		/* fall into downgrade */

	case LK_DOWNGRADE:
		atomic_lock(&lkp->lk_interlock);
		if (lkp->lk_lockholder != pid || lkp->lk_exclusivecount == 0)
			panic("lockmgr: not holding exclusive lock");
		lkp->lk_sharecount += lkp->lk_exclusivecount;
		lkp->lk_exclusivecount = 0;
		lkp->lk_flags &= ~LK_HAVE_EXCL;
		lkp->lk_lockholder = LK_NOPROC;
		if (lkp->lk_flags & LK_WAITING) {
			lkp->lk_flags &= ~LK_WAITING;
			wakeup(lkp);
		}
		atomic_unlock(&lkp->lk_interlock);
		return (0);

	case LK_UPGRADE:
		/*
		 * Upgrade a shared lock to an exclusive one. If another
		 * shared lock has already requested an upgrade to an
		 * exclusive lock, our shared lock is released and an
		 * exclusive lock is requested (which will be granted
		 * after the upgrade).
		 */
		atomic_lock(&lkp->lk_interlock);
		if (lkp->lk_lockholder == pid || lkp->lk_sharecount <= 0)
			panic("lockmgr: upgrade exclusive lock");
		/*
		 * If we are just polling, check to see if we will block.
		 */
		if ((extflags & LK_NOWAIT) &&
		    ((lkp->lk_flags & LK_WANT_UPGRADE) ||
		     lkp->lk_sharecount > 1)) {
			atomic_unlock(&lkp->lk_interlock);
			return (EBUSY);
		}
		lkp->lk_sharecount--;
		if ((lkp->lk_flags & LK_WANT_UPGRADE) == 0) {
			/*
			 * We are first shared lock to request an upgrade, so
			 * request upgrade and wait for the shared count to
			 * drop to zero, then take exclusive lock.
			 */
			lkp->lk_flags |= LK_WANT_UPGRADE;
			ACQUIRE(lkp, error, extflags, lkp->lk_sharecount);
			lkp->lk_flags &= ~LK_WANT_UPGRADE;
			if (error) {
				atomic_unlock(&lkp->lk_interlock);
				return (error);
			}
			lkp->lk_flags |= LK_HAVE_EXCL;
			lkp->lk_lockholder = pid;
			if (lkp->lk_exclusivecount != 0)
				panic("lockmgr: non-zero exclusive count");
			lkp->lk_exclusivecount = 1;
			atomic_unlock(&lkp->lk_interlock);
			return (0);
		}
		/*
		 * Someone else has requested upgrade. Release our shared
		 * lock, awaken upgrade requestor if we are the last shared
		 * lock, then request an exclusive lock.
		 */
		if (lkp->lk_sharecount == 0 && (lkp->lk_flags & LK_WAITING)) {
			lkp->lk_flags &= ~LK_WAITING;
			wakeup(lkp);
		}
		atomic_unlock(&lkp->lk_interlock);
		/* fall into exclusive request */

	case LK_EXCLUSIVE:
		atomic_lock(&lkp->lk_interlock);
		if (lkp->lk_lockholder == pid) {
			/*
			 *	Recursive lock.
			 */
			if ((extflags & LK_CANRECURSE) == 0)
				panic("lockmgr: locking against myself");
			lkp->lk_exclusivecount++;
			atomic_unlock(&lkp->lk_interlock);
			return (0);
		}
		/*
		 * If we are just polling, check to see if we will sleep.
		 */
		if ((extflags & LK_NOWAIT) && ((lkp->lk_flags &
		     (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE)) ||
		     lkp->lk_sharecount != 0)) {
			atomic_unlock(&lkp->lk_interlock);
			return (EBUSY);
		}
		/*
		 * Try to acquire the want_exclusive flag.
		 */
		ACQUIRE(lkp, error, extflags, lkp->lk_flags &
		    (LK_HAVE_EXCL | LK_WANT_EXCL));
		if (error) {
			atomic_unlock(&lkp->lk_interlock);
			return (error);
		}
		lkp->lk_flags |= LK_WANT_EXCL;
		/*
		 * Wait for shared locks and upgrades to finish.
		 */
		ACQUIRE(lkp, error, extflags, lkp->lk_sharecount != 0 ||
		       (lkp->lk_flags & LK_WANT_UPGRADE));
		lkp->lk_flags &= ~LK_WANT_EXCL;
		if (error) {
			atomic_unlock(&lkp->lk_interlock);
			return (error);
		}
		lkp->lk_flags |= LK_HAVE_EXCL;
		lkp->lk_lockholder = pid;
		if (lkp->lk_exclusivecount != 0)
			panic("lockmgr: non-zero exclusive count");
		lkp->lk_exclusivecount = 1;
		atomic_unlock(&lkp->lk_interlock);
		return (0);

	case LK_RELEASE:
		atomic_lock(&lkp->lk_interlock);
		if (lkp->lk_exclusivecount != 0) {
			lkp->lk_exclusivecount--;
			if (lkp->lk_exclusivecount == 0) {
				lkp->lk_flags &= ~LK_HAVE_EXCL;
				lkp->lk_lockholder = LK_NOPROC;
			}
		} else if (lkp->lk_sharecount != 0)
			lkp->lk_sharecount--;
		if (lkp->lk_flags & LK_WAITING) {
			lkp->lk_flags &= ~LK_WAITING;
			wakeup(lkp);
		}
		atomic_unlock(&lkp->lk_interlock);
		return (0);

	default:
		panic("lockmgr: unknown locktype request %d",
		    flags & LK_TYPE_MASK);
	}
}