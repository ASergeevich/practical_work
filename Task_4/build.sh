rm -r src/*
rm -r build/*
mkdir src
mkdir build

asn1c RRCv11.asn1 -D src -no-gen-OER -fcompound-names -no-gen-example -fno-include-deps

gcc -Isrc -o build/client client.c connection.c src/*.c -DPDU=Rectangle -DASN_DISABLE_OER_SUPPORT -lsctp
gcc -Isrc -o build/server server.c connection.c src/*.c -DPDU=Rectangle -DASN_DISABLE_OER_SUPPORT -lsctp