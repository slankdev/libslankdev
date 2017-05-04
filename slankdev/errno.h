
#pragma once

#include <errno.h>
#include <slankdev/exception.h>


namespace slankdev {

inline const char* errno2str(int e)
{
  switch (e) {
    case -EPERM          :   return "(EPERM)          Operation not permitted";
    case -ENOENT         :   return "(ENOENT)         No such file or directory";
    case -ESRCH          :   return "(ESRCH)          No such process";
    case -EINTR          :   return "(EINTR)          Interrupted system call";
    case -EIO            :   return "(EIO)            I/O error";
    case -ENXIO          :   return "(ENXIO)          No such device or address";
    case -E2BIG          :   return "(E2BIG)          Argument list too long";
    case -ENOEXEC        :   return "(ENOEXEC)        Exec format error";
    case -EBADF          :   return "(EBADF)          Bad file number";
    case -ECHILD         :   return "(ECHILD)         No child processes";
    case -EAGAIN         :   return "(EAGAIN)         Try again";
    case -ENOMEM         :   return "(ENOMEM)         Out of memory";
    case -EACCES         :   return "(EACCES)         Permission denied";
    case -EFAULT         :   return "(EFAULT)         Bad address";
    case -ENOTBLK        :   return "(ENOTBLK)        Block device required";
    case -EBUSY          :   return "(EBUSY)          Device or resource busy";
    case -EEXIST         :   return "(EEXIST)         File exists";
    case -EXDEV          :   return "(EXDEV)          Cross-device link";
    case -ENODEV         :   return "(ENODEV)         No such device";
    case -ENOTDIR        :   return "(ENOTDIR)        Not a directory";
    case -EISDIR         :   return "(EISDIR)         Is a directory";
    case -EINVAL         :   return "(EINVAL)         Invalid argument";
    case -ENFILE         :   return "(ENFILE)         File table overflow";
    case -EMFILE         :   return "(EMFILE)         Too many open files";
    case -ENOTTY         :   return "(ENOTTY)         Not a typewriter";
    case -ETXTBSY        :   return "(ETXTBSY)        Text file busy";
    case -EFBIG          :   return "(EFBIG)          File too large";
    case -ENOSPC         :   return "(ENOSPC)         No space left on device";
    case -ESPIPE         :   return "(ESPIPE)         Illegal seek";
    case -EROFS          :   return "(EROFS)          Read-only file system";
    case -EMLINK         :   return "(EMLINK)         Too many links";
    case -EPIPE          :   return "(EPIPE)          Broken pipe";
    case -EDOM           :   return "(EDOM)           Math argument out of domain of func";
    case -ERANGE         :   return "(ERANGE)         Math result not representable";
    case -EDEADLK        :   return "(EDEADLK)        Resource deadlock would occur";
    case -ENAMETOOLONG   :   return "(ENAMETOOLONG)   File name too long";
    case -ENOLCK         :   return "(ENOLCK)         No record locks available";
    case -ENOSYS         :   return "(ENOSYS)         Function not implemented";
    case -ENOTEMPTY      :   return "(ENOTEMPTY)      Directory not empty";
    case -ELOOP          :   return "(ELOOP)          Too many symbolic links encountered";
    case -ENOMSG         :   return "(ENOMSG)         No message of desired type";
    case -EIDRM          :   return "(EIDRM)          Identifier removed";
    case -ECHRNG         :   return "(ECHRNG)         Channel number out of range";
    case -EL2NSYNC       :   return "(EL2NSYNC)       Level 2 not synchronized";
    case -EL3HLT         :   return "(EL3HLT)         Level 3 halted";
    case -EL3RST         :   return "(EL3RST)         Level 3 reset";
    case -ELNRNG         :   return "(ELNRNG)         Link number out of range";
    case -EUNATCH        :   return "(EUNATCH)        Protocol driver not attached";
    case -ENOCSI         :   return "(ENOCSI)         No CSI structure available";
    case -EL2HLT         :   return "(EL2HLT)         Level 2 halted";
    case -EBADE          :   return "(EBADE)          Invalid exchange";
    case -EBADR          :   return "(EBADR)          Invalid request descriptor";
    case -EXFULL         :   return "(EXFULL)         Exchange full";
    case -ENOANO         :   return "(ENOANO)         No anode";
    case -EBADRQC        :   return "(EBADRQC)        Invalid request code";
    case -EBADSLT        :   return "(EBADSLT)        Invalid slot";
    case -EBFONT         :   return "(EBFONT)         Bad font file format";
    case -ENOSTR         :   return "(ENOSTR)         Device not a stream";
    case -ENODATA        :   return "(ENODATA)        No data available";
    case -ETIME          :   return "(ETIME)          Timer expired";
    case -ENOSR          :   return "(ENOSR)          Out of streams resources";
    case -ENONET         :   return "(ENONET)         Machine is not on the network";
    case -ENOPKG         :   return "(ENOPKG)         Package not installed";
    case -EREMOTE        :   return "(EREMOTE)        Object is remote";
    case -ENOLINK        :   return "(ENOLINK)        Link has been severed";
    case -EADV           :   return "(EADV)           Advertise error";
    case -ESRMNT         :   return "(ESRMNT)         Srmount error";
    case -ECOMM          :   return "(ECOMM)          Communication error on send";
    case -EPROTO         :   return "(EPROTO)         Protocol error";
    case -EMULTIHOP      :   return "(EMULTIHOP)      Multihop attempted";
    case -EDOTDOT        :   return "(EDOTDOT)        RFS specific error";
    case -EBADMSG        :   return "(EBADMSG)        Not a data message";
    case -EOVERFLOW      :   return "(EOVERFLOW)      Value too large for defined data type";
    case -ENOTUNIQ       :   return "(ENOTUNIQ)       Name not unique on network";
    case -EBADFD         :   return "(EBADFD)         File descriptor in bad state";
    case -EREMCHG        :   return "(EREMCHG)        Remote address changed";
    case -ELIBACC        :   return "(ELIBACC)        Can not access a needed shared library";
    case -ELIBBAD        :   return "(ELIBBAD)        Accessing a corrupted shared library";
    case -ELIBSCN        :   return "(ELIBSCN)        .lib section in a.out corrupted";
    case -ELIBMAX        :   return "(ELIBMAX)        Attempting to link in too many shared libraries";
    case -ELIBEXEC       :   return "(ELIBEXEC)       Cannot exec a shared library directly";
    case -EILSEQ         :   return "(EILSEQ)         Illegal byte sequence";
    case -ERESTART       :   return "(ERESTART)       Interrupted system call should be restarted";
    case -ESTRPIPE       :   return "(ESTRPIPE)       Streams pipe error";
    case -EUSERS         :   return "(EUSERS)         Too many users";
    case -ENOTSOCK       :   return "(ENOTSOCK)       Socket operation on non-socket";
    case -EDESTADDRREQ   :   return "(EDESTADDRREQ)   Destination address required";
    case -EMSGSIZE       :   return "(EMSGSIZE)       Message too long";
    case -EPROTOTYPE     :   return "(EPROTOTYPE)     Protocol wrong type for socket";
    case -ENOPROTOOPT    :   return "(ENOPROTOOPT)    Protocol not available";
    case -EPROTONOSUPPORT:   return "(EPROTONOSUPPORT)Protocol not supported";
    case -ESOCKTNOSUPPORT:   return "(ESOCKTNOSUPPORT)Socket type not supported";
    case -EOPNOTSUPP     :   return "(EOPNOTSUPP)     Operation not supported on transport endpoint";
    case -EPFNOSUPPORT   :   return "(EPFNOSUPPORT)   Protocol family not supported";
    case -EAFNOSUPPORT   :   return "(EAFNOSUPPORT)   Address family not supported by protocol";
    case -EADDRINUSE     :   return "(EADDRINUSE)     Address already in use";
    case -EADDRNOTAVAIL  :   return "(EADDRNOTAVAIL)  Cannot assign requested address";
    case -ENETDOWN       :   return "(ENETDOWN)       Network is down";
    case -ENETUNREACH    :   return "(ENETUNREACH)    Network is unreachable";
    case -ENETRESET      :   return "(ENETRESET)      Network dropped connection because of reset";
    case -ECONNABORTED   :   return "(ECONNABORTED)   Software caused connection abort";
    case -ECONNRESET     :   return "(ECONNRESET)     Connection reset by peer";
    case -ENOBUFS        :   return "(ENOBUFS)        No buffer space available";
    case -EISCONN        :   return "(EISCONN)        Transport endpoint is already connected";
    case -ENOTCONN       :   return "(ENOTCONN)       Transport endpoint is not connected";
    case -ESHUTDOWN      :   return "(ESHUTDOWN)      Cannot send after transport endpoint shutdown";
    case -ETOOMANYREFS   :   return "(ETOOMANYREFS)   Too many references: cannot splice";
    case -ETIMEDOUT      :   return "(ETIMEDOUT)      Connection timed out";
    case -ECONNREFUSED   :   return "(ECONNREFUSED)   Connection refused";
    case -EHOSTDOWN      :   return "(EHOSTDOWN)      Host is down";
    case -EHOSTUNREACH   :   return "(EHOSTUNREACH)   No route to host";
    case -EALREADY       :   return "(EALREADY)       Operation already in progress";
    case -EINPROGRESS    :   return "(EINPROGRESS)    Operation now in progress";
    case -ESTALE         :   return "(ESTALE)         Stale file handle";
    case -EUCLEAN        :   return "(EUCLEAN)        Structure needs cleaning";
    case -ENOTNAM        :   return "(ENOTNAM)        Not a XENIX named type file";
    case -ENAVAIL        :   return "(ENAVAIL)        No XENIX semaphores available";
    case -EISNAM         :   return "(EISNAM)         Is a named type file";
    case -EREMOTEIO      :   return "(EREMOTEIO)      Remote I/O error";
    case -EDQUOT         :   return "(EDQUOT)         Quota exceeded";
    case -ENOMEDIUM      :   return "(ENOMEDIUM)      No medium found";
    case -EMEDIUMTYPE    :   return "(EMEDIUMTYPE)    Wrong medium type";
    case -ECANCELED      :   return "(ECANCELED)      Operation Canceled";
    case -ENOKEY         :   return "(ENOKEY)         Required key not available";
    case -EKEYEXPIRED    :   return "(EKEYEXPIRED)    Key has expired";
    case -EKEYREVOKED    :   return "(EKEYREVOKED)    Key has been revoked";
    case -EKEYREJECTED   :   return "(EKEYREJECTED)   Key was rejected by service";
    case -EOWNERDEAD     :   return "(EOWNERDEAD)     Owner died";
    case -ENOTRECOVERABLE:   return "(ENOTRECOVERABLE)State not recoverable";
    case -ERFKILL        :   return "(ERFKILL)        Operation not possible due to RF-kill";
    case -EHWPOISON      :   return "(EHWPOISON)      Memory page has hardware error";
    default: throw slankdev::exception("UNKNOWN ERRNO: maybe not exist?");
  }
}

} /* namespace slankdev */


