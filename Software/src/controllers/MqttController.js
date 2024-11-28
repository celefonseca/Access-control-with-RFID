const mqtt = require('mqtt');
const UserController = require('./UserController.js');
const AcessosController = require('./AcessosController');

/**
 * Controlador MQTT.
 */
class MqttController {
    constructor() {
        this.mqttClient = null;
        this.clientId = `mqtt_${Math.random().toString(16).slice(2)}`;
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
    }

    /**
     * Processa mensagens recebidas e valida o usuário no UserController.
     * @param topic Tópico da mensagem recebida.
     * @param message Mensagem recebida.
     * @param topicPublish Tópico para resposta.
     */
    handleIncomingMessage = (
        topic,
        messageRaw,
        topicPublish
    ) => {

        if (!this.mqttClient) {
            throw new Error("Cliente MQTT não inicializado.");
        }

        console.log(`Mensagem recebida no tópico ${topic}: ${messageRaw}`);

        let message = messageRaw;

        try {
            const parsedMessage = JSON.parse(messageRaw.toString());

            if (parsedMessage.senderId) {
                // Ignorar mensagens enviadas por este cliente
                if (parsedMessage.senderId === this.clientId) {
                    return;
                }
                // Substituir a mensagem com o campo `data` do JSON
                message = parsedMessage?.data;
            }
        } catch (error) {
            console.warn('Mensagem não é JSON válida, utilizando mensagem crua:', error);
        }

        this.processRequest(message, topicPublish);
    }

    processRequest = async (message, topicPublish) => {
        try {
            const userToWantAcess = await UserController.validateUser(message);

            if (userToWantAcess) {
                console.log('Usuário já existe no banco de dados. Registrando acesso...');

                await AcessosController.registerAcess(userToWantAcess.id);

                console.log(`Acesso registrado para o usuário de ID: '${userToWantAcess.id}`);

                const payload = JSON.stringify({
                    senderId: this.clientId,
                    data: '1'
                });

                this.mqttClient.publish(topicPublish, payload);
            } else {
                console.log('Usuario invalido.');

                const payload = JSON.stringify({
                    senderId: this.clientId,
                    data: '0'
                });
                this.mqttClient.publish(topicPublish, payload);
            }
        } catch (error) {
            console.error('Erro ao processar solicitação:', error);
        }
    }
}

module.exports = MqttController;