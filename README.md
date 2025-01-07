# **Controle de Sistema Auxiliar de Cadeira de Rodas Motorizada**

Este projeto implementa um sistema de controle remoto para cadeiras de rodas motorizadas, fornecendo uma experiência de mobilidade segura, eficiente e inovadora. Ele utiliza o controlador VESC 6.9 para gerenciar motores BLDC e integra o microcontrolador ESP32 para comunicação, monitoramento em tempo real e funcionalidades avançadas de segurança.

---

## **Índice**
1. [Descrição Geral](#descrição-geral)
2. [Funcionalidades](#funcionalidades)
3. [Arquitetura do Projeto](#arquitetura-do-projeto)
4. [Instruções de Instalação](#instruções-de-instalação)
5. [Como Usar](#como-usar)
6. [Estrutura do Código](#estrutura-do-código)
7. [Componentes e Materiais Utilizados](#componentes-e-materiais-utilizados)
8. [Melhorias Futuras](#melhorias-futuras)
9. [Licença](#licença)

---

## **Descrição Geral**

O sistema é composto por hardware e software que trabalham de forma integrada para controlar o motor BLDC, monitorar a bateria de potência e fornecer feedback contínuo ao usuário. Ele permite controle por Bluetooth e USB, exibindo informações no display OLED, como:
- Nível de bateria.
- Velocidade atual.
- Modo de operação ativo.
- Status de eficiência operacional.

Além disso, inclui um **módulo de emergência** que pode desligar rapidamente o sistema em situações críticas.

---

## **Funcionalidades**

1. **Controle de Velocidade Dinâmico**:
   - Ajuste de velocidade via encoder rotativo.
   - Parada segura ao pressionar o encoder.

2. **Modos de Operação Inteligentes**:
   - **Modo Inoperante**: Sistema desativado.
   - **Modo Interno**: Limite de velocidade para ambientes fechados.
   - **Modo Externo**: Potência e velocidade aumentadas para terrenos externos.

3. **Monitoramento de Bateria em Tempo Real**:
   - Dados detalhados de carga, tensão, corrente e temperatura da bateria via Bluetooth.

4. **Conexão Flexível**:
   - Comunicação via Bluetooth ou USB com fallback automático.

5. **Feedback Visual e Sonoro**:
   - Display OLED para informações do sistema.
   - Alertas sonoros para emparelhamento, ajustes e falhas.

6. **Segurança Avançada**:
   - Módulo de emergência para desligamento instantâneo.
   - Proteções contra sobrecarga e superaquecimento.

---

## **Arquitetura do Projeto**

### **Hardware**
- **Controlador VESC 6.9**: Gerencia torque, corrente e velocidade do motor BLDC.
- **ESP32 DEVKITV1**: Microcontrolador para processamento e comunicação.
- **Display OLED (128x64 px)**: Interface visual para o usuário.
- **Encoder Rotativo KY-040**: Controle de velocidade.
- **Módulo de Emergência**: Desliga o sistema em situações críticas.
- **BMS**: Monitora a bateria de potência.

### **Software**
- **Firmware ESP32**: Implementa a comunicação entre VESC, BMS e controle remoto.
- **Bibliotecas Importantes**:
  - `VescUart.cpp`: Gerencia a comunicação UART com o VESC.
  - `datatypes.h`: Define tipos e estruturas utilizadas pelo firmware.
  - 'u82g': Responsável pelo gerenciamento do display e todas sua responsividade
---

## **Instruções de Instalação**

### **Pré-requisitos**
- Arduino IDE ou PlatformIO instalado.
- Biblioteca `esp32` adicionada ao ambiente.
- Hardware configurado conforme esquemático.

### **Passos**
1. Clone este repositório:
   ```bash
   git clone https://github.com/usuario/projeto-cadeira-rodas.git
