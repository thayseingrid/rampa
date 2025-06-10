const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

// Substitua pela sua porta
const port = new SerialPort({
    path: '/dev/ttyUSB0',  // <-- Altere aqui
    baudRate: 9600,
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

let dadosRecebidos = []; // Armazena os GET recebidos do Arduino

port.on('open', () => {
    console.log('Serial port aberta');
});

port.on('error', (err) => {
    console.error('Erro na serial:', err.message);
});

// Lê e processa os dados do Arduino
parser.on('data', (linha) => {
    linha = linha.trim();

    if (linha.startsWith('GET')) {
        console.log('Arduino >>', linha);
        const partes = linha.split(' ');

        for (let i = 1; i < partes.length; ++i) {
            const tempo = parseInt(partes[i], 10);

            if (!isNaN(tempo)) {
                dadosRecebidos.push(tempo);
            }
        }

        console.log("recebeu get");
    } else if (linha.startsWith("NODATA")) {
        dadosRecebidos = [];
    } else {
        console.log('Arduino >>', linha);
    }
});

function enviarComando(cmd) {
  if (port.writable) {
    port.write(cmd + '\n', (err) => {
      if (err) {
        console.error('Erro ao escrever:', err.message);
      } else {
        console.log('Comando enviado:', cmd);
      }
    });
  }
}

function obterDados() {
  return dadosRecebidos;
}

function limparDados() {
  dadosRecebidos.length = 0;
}

module.exports = {
  enviarComando,
  obterDados,
  limparDados,
};

