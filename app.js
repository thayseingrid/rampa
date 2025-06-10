const express = require('express');
const { enviarComando, obterDados, limparDados } = require('./serial');

const app = express();
const PORT = 3000;

const path = require('path');
app.use(express.static(path.join(__dirname)));

app.use(express.json());

app.post('/reset', (req, res) => {
  enviarComando('RESET');
  limparDados();
  res.json({ status: 'Comando RESET enviado ao Arduino e dados limpos' });
});

app.get('/dados', (req, res) => {
  const dados = obterDados();
  res.json(dados);
});

app.get('/testar', (req, res) => {
    enviarComando('TESTAR');
    res.json({ ok: "Ok!"});
});

app.get('/claser', (req, res) => {
    enviarComando('CLASER');
    res.json({ ok: "Ok!"});
});

app.get('/slaser', (req, res) => {
    enviarComando('SLASER');
    res.json({ ok: "Ok!"});
});

app.get('/capturar', (req, res) => {
    enviarComando('CAPTURAR');
    res.json({ ok: "Ok!"});
});

app.get('/capturar2', (req, res) => {
    enviarComando('RESET');
    enviarComando('LASER');
    enviarComando('CAPTURAR');
    res.json({ ok: "Ok!"});
});


app.listen(PORT, () => {
  console.log(`Servidor backend rodando em http://localhost:${PORT}`);
});

