# delaymgr
webrtc delaymanger python



# 使用说明



```python
python setup.py build
python setup.py install
python setup.py clean
```

## 示例

```python
import pydetector
import random

def main(args):
    dd = pydetector.WebDete(20)
    fs_hz_ = 48000
    packet_length_samples = 960
    main_timestamp = 0
    timestamp_ = main_timestamp
    sequenceNumber = 100

    count1 = 0
    count2 = 0
    count3 = 0
    count4 = 0
    for i in range(10000):
        random_num = random.randint(0, 200)
        sec = random_num // 10
        if sec >= 4:
            sec = 1
            count1 = count1 + 1
        elif sec >= 3:
            sec = 6
            count2 = count2 + 1
        elif sec >=2:
            sec = sec + 1
            count3 = count3 + 1
        elif sec >= 1:
            sec = 1
            count4 = count4 + 1
        else:
            sec = 1
        
        for j in range(sec):
            print("sec:",sec+1," j:",j)
            dd.tick(2)

        main_timestamp = main_timestamp + packet_length_samples
        sequenceNumber = sequenceNumber + 1

        dd.update(sequenceNumber, main_timestamp, fs_hz_)

    print(dd.target_level())
    print(dd.histogram())

if __name__ == '__main__':
    main(1)
```



