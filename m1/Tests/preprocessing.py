import pandas as pd

df = pd.read_csv('csv/WalkingAround.csv')
content = df.to_string()
print(content, file=open('csv_as_text/WalkingAround.txt', 'w'))

df = pd.read_csv('csv/Turntable1.csv')
content = df.to_string()
print(content, file=open('csv_as_text/Turntable1.txt', 'w'))

df = pd.read_csv('csv/Turntable2.csv')
content = df.to_string()
print(content, file=open('csv_as_text/Turntable2.txt', 'w'))