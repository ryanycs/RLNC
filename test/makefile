all: test_decoder test_encoder test_galois

test_galois:
	gcc -o test_galois test_galois.c ../src/galois.c ../third_party/gf.c

test_encoder: 
	gcc -o test_encoder test_encoder.c ../src/encoder.c ../src/galois.c

test_decoder:
	gcc -o test_decoder test_decoder.c ../src/encoder.c ../src/decoder.c ../src/galois.c

clean:
	@if [ -f "test_encoder" ]; then rm test_encoder; fi
	@if [ -f "test_galois" ]; then rm test_galois; fi
	@if [ -f "test_decoder" ]; then rm test_decoder; fi