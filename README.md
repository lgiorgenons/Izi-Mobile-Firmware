# Controle de Sistema Auxiliar de Cadeira de Rodas

## Descrição do Projeto
Este projeto implementa um sistema auxiliar para cadeiras de rodas motorizadas utilizando o controlador VESC 6.9. Ele oferece controle preciso do motor BLDC, monitoramento em tempo real da bateria, e recursos de segurança avançados.

O sistema utiliza um microcontrolador ESP32 para gerenciar a comunicação via Bluetooth e USB, conectando o controle remoto ao VESC e à BMS (Battery Management System). Além disso, conta com um módulo de emergência para desligamento rápido em situações críticas.

## Funcionalidades Principais
- **Conexão via Bluetooth e USB**: Comunicação flexível com o VESC e BMS.
- **Monitoramento de Bateria**: Exibição de tensão, corrente e temperatura da bateria no display OLED.
- **Modos de Operação**: Configurações internas e externas, com limite de velocidade baseado no ambiente.
- **Controle de Velocidade**: Encoder rotativo KY-040 para ajustes dinâmicos.
- **Segurança Avançada**: Módulo de emergência e proteção da bateria contra sobrecarga.

## Componentes Principais
- **ESP32**: Gerencia comunicação e processamento de dados.
- **VESC 6.9**: Controlador de motor BLDC.
- **Display OLED (128x64 px)**: Exibe status do sistema e informações da bateria.
- **Encoder Rotativo KY-040**: Controla velocidade e paradas.
- **Módulo de Emergência**: Garante desligamento seguro.

## Estrutura do Código
- `VescUart.cpp`: Implementa a comunicação UART com o controlador VESC.
- `datatypes.h`: Define tipos e estruturas utilizados no firmware VESC.

### Interface

### Comunicação

## Instalação
1. Clone este repositório:
   ```bash
   git clone https://github.com/usuario/projeto-cadeira-rodas.git
