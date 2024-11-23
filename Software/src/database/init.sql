-- Criação da tabela 'usuarios'
CREATE TABLE IF NOT EXISTS usuarios (
  id VARCHAR(255) PRIMARY KEY,
  nome VARCHAR(255) NOT NULL
);

-- Inserção de dados iniciais na tabela 'usuarios'
INSERT INTO usuarios (id, nome) VALUES
('1', 'Alice'),
('2', 'Bob'),
('3', 'Charlie');

-- Criação da tabela 'acessos'
CREATE TABLE IF NOT EXISTS acessos (
  id SERIAL PRIMARY KEY,        -- Identificador único do evento
  usuario_id VARCHAR(255) NOT NULL, -- ID do usuário relacionado
  evento_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- Data e hora do evento
  CONSTRAINT fk_usuario FOREIGN KEY (usuario_id) REFERENCES usuarios (id)
);