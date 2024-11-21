 const broker = process.env.MQTT_BROKER || 'mqtt://broker.hivemq.com';
 const topicSubscribe = process.env.MQTT_TOPIC_SUBSCRIBE || 'test/topic';
 const topicPublish = process.env.MQTT_TOPIC_PUBLISH || 'test/response';


module.exports = { broker, topicSubscribe, topicPublish };