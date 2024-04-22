# EMG-recording-module
Design for the hardware of EMG recording module ||
Code for system: STM8S, bluetooth HC-05, EMG recording module || 
Model 3D with cases
# Hardware of EMG recording module
The module use a system filter that enough to clean the signal input before going to ADC. The ADC have I2C interface and programmable gain. The I2C is useful for multiple devices if necessary, and the programmable gain use for auto adjustment function.
![image](https://github.com/Ngoc-Phu-Nguyen/EMG-recording-module/assets/167606858/3327e0c2-1637-467c-a296-a321152d9cee)

Fig1. Scheme of circuit 

![image](https://github.com/Ngoc-Phu-Nguyen/EMG-recording-module/assets/167606858/44b049b0-34c2-428e-8e17-6ff846eb9e5c)
![image](https://github.com/Ngoc-Phu-Nguyen/EMG-recording-module/assets/167606858/687bb0ca-f5f9-4752-8612-ac4acf11f1bb)

Fig2. 3D view of circuit

![image](https://github.com/Ngoc-Phu-Nguyen/EMG-recording-module/assets/167606858/e004eac3-4560-4483-9917-e861d33dcd6b)
![image](https://github.com/Ngoc-Phu-Nguyen/EMG-recording-module/assets/167606858/a89d26a2-8852-4482-8f2b-0c47af93fad9)

Fig3. 3D view with cases

![image](https://github.com/Ngoc-Phu-Nguyen/EMG-recording-module/assets/167606858/4862e8de-a274-4f45-a0b3-f4eacfdc6938)

Fig4. The real one - this module includes two main parts: Transmitter and receiver. The transmitter includes: STM8S, battery, EMG recording and HC-05. The receiver includes: black pill STM32, HC-05. 

![image](https://github.com/Ngoc-Phu-Nguyen/EMG-recording-module/assets/167606858/a62b4619-b6bb-43ef-abd1-c7eeff61201e)

Fig 5. Result of signal EMG from the module - Signal is quite clean. There is almost no noise and interference. The sampling rate of signal is 3ms.
