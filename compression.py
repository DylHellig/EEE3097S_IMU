import zlib
import time 

def compress(plaintext, level):
    comp = zlib.compress(plaintext.encode('utf-8'), level)
    return comp

def decompress(comp):
    decomp = zlib.decompress(comp)
    return decomp

def get_time():
    t = time.perf_counter()
    return t

def do_compression(filenames):
    for file in filenames:
        with open('csv_as_text/'+file) as f:
            original = f.readlines()

        compressed = []
        start = get_time()
        for line in original:
            comp = compress(line, 8)
            compressed.append(comp)
        stop = get_time()
        comp_time = round(stop-start, 4)

        with open('compressed/'+file+'_compressed.txt', 'wb') as f:
            f.writelines(compressed)

        decompressed = []
        start = get_time()
        for line in compressed:
            decomp = zlib.decompress(line)
            decompressed.append(decomp)
        stop = get_time()
        decomp_time = round(stop-start, 4)

        total_time = comp_time + decomp_time

        with open('decompressed/'+file+'_decompressed.txt', 'wb') as f:
            f.writelines(decompressed)


        with open('csv_as_text/'+file) as f:
            original_file = f.read()

        with open('decompressed/'+file+'_decompressed.txt') as f:
            decompressed_file = f.read()
        print(f'Checking decompression for {file}')
        if original_file==decompressed_file:
            print('All data retained after compression and decompression')
        else:
            print('Insufficient data retained after compression and decompression')

        print(f'Compression Execution Time {comp_time} seconds')
        print(f'Decompression Execution Time {decomp_time} seconds')
        print(f'Total Execution Time {total_time} seconds')

        print('-------------------------------------')

if __name__ == '__main__':
    filenames = ['Turntable1.txt', 'Turntable2.txt', 'WalkingAround.txt']
    do_compression(filenames)