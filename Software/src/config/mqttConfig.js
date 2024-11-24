require('dotenv').config();

 const broker = process.env.MQTT_BROKER;
 const topicSubscribe = process.env.MQTT_TOPIC_SUBSCRIBE;
 const topicPublish = process.env.MQTT_TOPIC_PUBLISH;

 if (!broker || !topicSubscribe || !topicPublish) {
    console.error('Erro: Variáveis de ambiente não configuradas corretamente.');
    process.exit(1);
  }

module.exports = { broker, topicSubscribe, topicPublish };