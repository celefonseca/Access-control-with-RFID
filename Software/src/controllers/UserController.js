const UserRepository = require('../repository/UserRepository.js');


const validateUser = async (id, mqttClient, topicPublish) => {
    try {
      const existingUser = await UserRepository.getUserById(id);

      if (existingUser) {
        console.log('Usuario já existe no banco de dados. Enviando resposta...');
        mqttClient.publish(topicPublish, `Usuario de Id:'${id}' já existe no banco.`);
      } else {
        console.log('Usuario invalido.');
      }
    } catch (err) {
      console.error('Erro ao validar usuario:', err);
    }
  }

  module.exports={validateUser};