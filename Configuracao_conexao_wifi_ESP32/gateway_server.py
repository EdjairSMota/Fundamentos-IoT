from flask import Flask, request

app = Flask(__name__)

@app.route('/datalogger', methods=['POST'])
def receive_data():
    if request.method == 'POST':
        data = request.data.decode('utf-8')
        with open('datalog_pi.txt', 'a') as f:
            f.write(data + '\n')
        print(f"Dados recebidos: {data}")
        return "Dados recebidos com sucesso!", 200
    return "Método não permitido", 405

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)


    
