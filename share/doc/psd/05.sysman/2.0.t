.\" Copyright (c) 1983, 1993
.\"	The Regents of the University of California.  All rights reserved.
.\"
.\" Redistribution and use in source and binary forms, with or without
.\" modification, are permitted provided that the following conditions
.\" are met:
.\" 1. Redistributions of source code must retain the above copyright
.\"    notice, this list of conditions and the following disclaimer.
.\" 2. Redistributions in binary form must reproduce the above copyright
.\"    notice, this list of conditions and the following disclaimer in the
.\"    documentation and/or other materials provided with the distribution.
.\" 3. All advertising materials mentioning features or use of this software
.\"    must display the following acknowledgement:
.\"	This product includes software developed by the University of
.\"	California, Berkeley and its contributors.
.\" 4. Neither the name of the University nor the names of its contributors
.\"    may be used to endorse or promote products derived from this software
.\"    without specific prior written permission.
.\"
.\" THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
.\" ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
.\" IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
.\" ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
.\" FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
.\" DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
.\" OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
.\" HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
.\" LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
.\" OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
.\" SUCH DAMAGE.
.\"
.\"	@(#)2.0.t	8.2 (Berkeley) 05/16/94
.\"
.Sh 1 "System facilities
.LP
The system abstractions described are:
.IP "Directory contexts
.br
A directory context is a position in the filesystem name
space.  Operations on files and other named objects in a filesystem are
always specified relative to such a context.
.IP "Files
.br
Files are used to store uninterpreted sequence of bytes on which
random access \fIreads\fP and \fIwrites\fP may occur.
Pages from files may also be mapped into process address space.
A directory may be read as a file,
though it is usually accessed using
.Fn getdirentries
(see section
.Xr 2.2.3.1 ).
.IP "Communications domains
.br
A communications domain represents
an interprocess communications environment, such as the communications
facilities of the 4.4BSD system,
communications in the INTERNET, or the resource sharing protocols
and access rights of a resource sharing system on a local network.
.IP "Sockets
.br
A socket is an endpoint of communication and the focal
point for IPC in a communications domain.  Sockets may be created in pairs,
or given names and used to rendezvous with other sockets
in a communications domain, accepting connections from these
sockets or exchanging messages with them.  These operations model
a labeled or unlabeled communications graph, and can be used in a
wide variety of communications domains.  Sockets can have different
\fItypes\fP\| to provide different semantics of communication,
increasing the flexibility of the model.
.IP "Terminals and other devices
.br
Devices include
terminals, providing input editing and interrupt generation
and output flow control and editing, magnetic tapes,
disks and other peripherals.  They often support the generic
\fIread\fP and \fIwrite\fP operations as well as a number of \fIioctl\fP\|s.
.IP "Processes
.br
Process descriptors provide facilities for control and debugging of
other processes.
