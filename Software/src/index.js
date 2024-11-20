const mqtt = require('mqtt');
const { Client } = require('pg');

// Configuração do broker MQTT
const broker = 'mqtt://broker.hivemq.com';
const topicSubscribe = 'test/topic';
const topicPublish = 'test/response';

// Conexão com o banco de dados PostgreSQL
const dbClient = new Client({
  host: 'localhost',
  database: 'meu_banco',
  user: 'postgres',
  password: 'senha_segura',
  port: 5432
});

dbClient.connect()
  .then(() => console.log('Conectado ao banco de dados PostgreSQL'))
  .catch(err => console.error('Erro ao conectar ao banco de dados:', err));

// Conexão com o broker MQTT
const mqttClient = mqtt.connect(broker);

mqttClient.on('connect', () => {
  console.log('Conectado ao broker MQTT');
  mqttClient.subscribe(topicSubscribe, (err) => {
    if (!err) {
      console.log(`Inscrito no tópico: ${topicSubscribe}`);
    } else {
      console.error('Erro ao subscrever ao tópico:', err);
    }
  });
});

// Callback para quando uma mensagem é recebida no tópico de inscrição
mqttClient.on('message', (topic, message) => {
  const mensagemRecebida = message.toString();
  console.log(`Mensagem recebida no tópico ${topic}: ${mensagemRecebida}`);

  // Verificar se a mensagem já existe no banco de dados
  dbClient.query('SELECT conteudo FROM mensagens WHERE conteudo = $1', [mensagemRecebida], (err, res) => {
    if (err) {
      console.error('Erro ao consultar o banco de dados:', err);
      return;
    }

    if (res.rows.length > 0) {
      console.log('Mensagem já existe no banco de dados. Enviando resposta...');
      // Publicar mensagem no outro tópico
      mqttClient.publish(topicPublish, `Mensagem '${mensagemRecebida}' já existe no banco.`);
    } else {
      console.log('Mensagem nova. Salvando no banco de dados.');
      // Inserir a nova mensagem no banco de dados
      dbClient.query('INSERT INTO mensagens (conteudo) VALUES ($1)', [mensagemRecebida], (err) => {
        if (err) {
          console.error('Erro ao inserir no banco de dados:', err);
        } else {
          console.log('Mensagem salva com sucesso no banco de dados.');
        }
      });
    }
  });
});

// Encerrar a conexão com o banco de dados ao finalizar o processo
process.on('exit', () => {
  dbClient.end();
  console.log('Conexão com o banco de dados encerrada.');
});