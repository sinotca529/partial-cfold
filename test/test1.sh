#!/usr/bin/bash
diff <(${LLVM15}/bin/opt --load-pass-plugin=../build/PartialCFold.so --passes="function(pcfold)" -S main.ll -o - ) out.ll
