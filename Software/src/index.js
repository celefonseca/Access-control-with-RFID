const MqttController = require('./controllers/MqttController');
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