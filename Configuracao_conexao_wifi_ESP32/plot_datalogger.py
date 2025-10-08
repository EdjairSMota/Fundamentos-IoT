import numpy as np
import matplotlib.pyplot as plt

file_path = 'datalog_pi.txt'

try:
    data = np.loadtxt(file_path, delimiter=',')
    time_ms = data[:, 0]
    temp_ds18b20 = data[:, 1]
    temp_ntc10k = data[:, 2]
    time_s = (time_ms - time_ms[0]) / 1000.0
    plt.figure(figsize=(10, 6))
    plt.plot(time_s, temp_ds18b20, label='DS18B20', color='red', marker='o', linestyle='-')
    plt.plot(time_s, temp_ntc10k, label='NTC10K', color='blue', marker='x', linestyle='--')
    plt.title('Temperatura dos Sensores')
    plt.xlabel('Tempo (s)')
    plt.ylabel('Temperatura (°C)')
    plt.legend()
    plt.grid(True)
    plt.show()

except FileNotFoundError:
    print(f"Erro: O arquivo '{file_path}' não foi encontrado.")
except Exception as e:
    print(f"Ocorreu um erro ao processar o arquivo: {e}")


    