## Encryption and Compression System Design for Ice Buoy IMU
<p>This repository contains the programs and tests for the software simulation and hardware validation for the system design.</p>

### Key Programs:

* Software_Validation:<br>
  * aes.py -> encryption/decryption simulation program
  * compression.py -> compression/decompression simulation program
  * system.py -> Overall system serial communication program
  * tests.ipynb -> Jupyter notebook for ATP validation
  * Turntable150.txt -> Sample data for software simulation

* Hardware_Validation <br>
  * Arduino: -> STM programs<br>
    * compression_test.ino -> Compression block tets on STM
    * encryption_test.ino -> Encryption block test on STM
    * imu_test.ino -> Full system program
    * known_string.ino -> Overall System test on STM
    * sensor_test.ino -> Sensor Validation Test
  * host_machine: -> Host Machine programs
    * Decompression_block:
      * decompression.py -> Decompression block program
    * Decryption_block
      * decrypt.cpp -> Decryption block program
    * post_processing:
      * post.py -> programe to create final txt/csv file ouput
  * m3_tests:
    * plots.ipynb -> Jupyter notebook for ATP validation
    

