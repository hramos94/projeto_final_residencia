import sys
import subprocess

# Comando que queremos executar
comando = ['make', 'test']

# Executando o comando no terminal
with open('test/scripts/test_capture.txt', 'w') as f:
    processo = subprocess.Popen(comando, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    
    # Lendo e capturando a saída do comando
    for linha in processo.stdout:
        print(linha, end='')  # Imprimindo no terminal
        f.write(linha)        # Escrevendo no arquivo

    # Lendo e capturando os erros, se houver
    for linha in processo.stderr:
        print(linha, end='', file=sys.stderr)  # Imprimindo no terminal em caso de erro
        f.write(linha)        # Escrevendo no arquivo

    # Esperando o processo terminar
    processo.wait()

print("A execução foi concluída. Veja o output em 'output.txt'.")