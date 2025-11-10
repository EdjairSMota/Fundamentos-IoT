import subprocess

line_protocol = 'temperatura_agua,sensor=DS18B20,local=tanque valor=27.3 1698931200000000000'

curl_cmd = [
    'curl', '-i', '-XPOST', 'http://localhost:8086/write?db=monitoramento_agua',
    '--data-binary', line_protocol
]

subprocess.run(curl_cmd)
