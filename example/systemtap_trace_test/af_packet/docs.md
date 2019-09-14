

# SystemtapでPF_PACKETをトレース

## アプローチ方法

単純にnet/packet/af_packet.cの関数が実行されるところにプローブを置いただけ。
LibPGENで簡単なパケット送信スクリプトを実装して、それを実行したところに
プローブポイントを仕掛けた。


## 使用したstpとパケット送信スクリプト

***stp***

```
#! /usr/bin/env stap

probe kernel.function("*@net/packet/af_packet.c").call {
  printf ("%15s -> %15s\n", thread_indent(4), probefunc())
}
probe kernel.function("*@net/packet/af_packet.c").return {
  printf ("%15s <- %15s\n", thread_indent(-4), probefunc())
}
```

***script***

```
#include <slankdev.h>
const char* dev = "lo";

int main()
{
    slankdev::safe_intfd fd;
    fd.open_if(dev);

    slankdev::hexdump("out", slankdev::raw, slankdev::rawlen);
    fd.write(slankdev::raw, slankdev::rawlen);
}
```

## ログ内容

```
     0 a.out(7879):    ->   packet_create
    19 a.out(7879):        -> register_prot_hook.part.48
    24 a.out(7879):        <-   packet_create
    28 a.out(7879):    <-   __sock_create
     0 a.out(7879):    ->    packet_ioctl
     5 a.out(7879):    <-   sock_do_ioctl
     0 a.out(7879):    ->     packet_bind
     3 a.out(7879):        -> packet_do_bind.part.63
     8 a.out(7879):            -> __unregister_prot_hook
 30362 a.out(7879):            <- packet_do_bind.part.63
 30373 a.out(7879):            -> register_prot_hook.part.48
 30377 a.out(7879):            <- packet_do_bind.part.63
 30379 a.out(7879):        <-     packet_bind
 30381 a.out(7879):    <-       SYSC_bind
     0 a.out(7879):    ->    packet_ioctl
     3 a.out(7879):    <-   sock_do_ioctl
     0 a.out(7879):    ->    packet_ioctl
     5 a.out(7879):    <-   sock_do_ioctl
     0 a.out(7879):    ->  packet_sendmsg
     7 a.out(7879):        -> packet_pick_tx_queue
     9 a.out(7879):        <-  packet_sendmsg
    18 a.out(7879):        ->      packet_rcv
    22 a.out(7879):        <- __netif_receive_skb_core
    27 a.out(7879):    <-    sock_sendmsg
     0 a.out(7879):    ->  packet_release
     2 a.out(7879):        -> __unregister_prot_hook
     4 a.out(7879):        <-  packet_release
 23931 a.out(7879):        -> packet_sock_destruct
 23940 a.out(7879):        <-     sk_destruct
 23944 a.out(7879):    <-    sock_release
```
