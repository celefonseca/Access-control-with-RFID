const mqtt = require('mqtt');
const UserController = require('./UserController.js');
const AcessosController = require('./AcessosController');
const User = require('../entities/User.js');

/**
 * Controlador MQTT.
 */
class MqttController {
    constructor() {
        this.mqttClient = null;
    }
    /**
     * Inicializa o cliente MQTT e conecta ao broker fornecido.
     * @param broker URL do broker MQTT
     */
    initializeMqttClient = (broker) => {
        this.mqttClient = mqtt.connect(broker);

        this.mqttClient.on("connect", () => {
            console.log(`Conectado ao broker MQTT: ${broker}`);
        });

        this.mqttClient.on("error", (err) => {
            console.error("Erro no cliente MQTT:", err);
        });
    }

    /**
     * Subscreve a um tópico específico.
     * @param topicSubscribe Tópico no qual o cliente irá se inscrever.
     */
    subscribeToTopic = (topicSubscribe) => {
        if (!this.mqttClient) {
            throw new Error("Cliente MQTT não inicializado.");
        }

        this.mqttClient.subscribe(topicSubscribe, (err) => {
            if (!err) {
                console.log(`Inscrito no tópico: ${topicSubscribe}`);
            } else {
                console.error("Erro ao subscrever ao tópico:", err);
            }
        });

        this.mqttClient.on("message", (topic, message) => {
            this.handleIncomingMessage(topic, message.toString(), topicSubscribe);
        });
    }

    /**
     * Processa mensagens recebidas e valida o usuário no UserController.
     * @param topic Tópico da mensagem recebida.
     * @param message Mensagem recebida.
     * @param topicPublish Tópico para resposta.
     */
    handleIncomingMessage = (
        topic,
        message,
        topicPublish
    ) => {
        console.log(`Mensagem recebida no tópico ${topic}: ${message}`);

        if (!this.mqttClient) {
            throw new Error("Cliente MQTT não inicializado.");
        }
        this.processRequest(message, topicPublish);
    }

    processRequest = async (message, topicPublish) => {
        try {
            const userToWantAcess = await UserController.validateUser(message, this.mqttClient, topicPublish);

            if (userToWantAcess) {
                console.log('Usuário já existe no banco de dados. Registrando acesso...');

                // Chama o método em AcessosController para registrar o acesso
                await AcessosController.registerAcess(userToWantAcess.id);

                // Publica uma mensagem no tópico indicando sucesso
                this.mqttClient.publish(topicPublish, `Acesso registrado para o usuário de ID: '${userToWantAcess.id}'.`);
            } else {
                console.log('Usuario invalido.');
                this.mqttClient.publish(topicPublish, 'Usuário inválido.');
            }
        } catch (error) {
            console.error('Erro ao processar solicitação:', error);
            this.mqttClient.publish(topicPublish, 'Erro ao processar a solicitação.');
        }
    }
}

module.exports = MqttController;