#!/bin/bash
qemu-system-x86_64 -cpu qemu64,vmx=on,sse4.2=on,sse4.1=on,aes=on,avx=on,f16c=on,pclmulqdq=on,avx2=on,vaes=on,vpclmulqdq=on -hda Image/x64BareBonesImage.qcow2 -m 512 
