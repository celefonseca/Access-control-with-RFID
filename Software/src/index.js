const express = require('express');
const bodyParser = require('body-parser');
const MqttController = require('./controllers/MqttController');
const UserController = require('./controllers/UserController.js');
const AcessosController = require('./controllers/AcessosController.js');
const path = require('path');
const  {broker, topicSubscribe, topicPublish } = require('./config/mqttConfig');

// Criando uma instância do MqttController
const mqttController = new MqttController();

// Inicializando o cliente MQTT
mqttController.initializeMqttClient(broker);

// Definindo o comportamento após conexão
mqttController.mqttClient?.on('connect', () => {
  console.log('Conectado ao broker MQTT');
  mqttController.subscribeToTopic(topicSubscribe);
});

// Definindo o comportamento ao receber mensagens
mqttController.mqttClient?.on('message', async (topic, message) => {
  await mqttController.handleIncomingMessage(topic, message.toString(), topicPublish);
});


const app = express();
const PORT = 3000;

// Middleware para interpretar JSON
app.use(bodyParser.json());

app.use(express.static(path.join(__dirname, '../public')));

app.post('/register', async (req, res) => {
  try {
    const { id, name } = req.body;

    if (!id || !name) {
      return res.status(400).json({ error: 'Id e nome são obrigatórios!' });
    }

    UserController.registerUser({ id, name });

    res.status(201).json({ message: 'Usuário cadastrado com sucesso!', data: { id, name } });
  } catch (error) {
    console.error('Erro ao cadastrar usuário:', error);
    res.status(500).json({ error: 'Erro interno do servidor.' });
  }
});

app.get('/access', async (req, res) => {
  try {
    const acessosComUsuarios = await AcessosController.listAccess();

    res.status(200).json(acessosComUsuarios);
  } catch (error) {
    console.error('Erro ao listar acessos:', error);
    res.status(500).json({ error: 'Erro interno do servidor.' });
  }
});

// Iniciando o servidor Express
app.listen(PORT, () => {
  console.log(`Servidor Express rodando na porta ${PORT}`);
});