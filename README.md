# README - Matriz de LEDs WS2812B com Raspberry Pi Pico (<https://youtu.be/p0gGDkFw-tQ>)

## Descrição
Este projeto controla uma matriz de LEDs WS2812B utilizando um Raspberry Pi Pico. O sistema permite a exibição de padrões numéricos (0 a 9) em uma matriz 5x5 de LEDs RGB. O usuário pode alternar entre os padrões utilizando dois botões físicos.

## Funcionalidades
- Exibição de padrões numéricos de 0 a 9 na matriz LED.
- Controle da matriz utilizando a biblioteca PIO.
- Alternação entre padrões com botões físicos.
- Implementação de debounce para evitar leituras errôneas dos botões.
- Pisca um LED RGB vermelho 5 vezes por segundo como indicador visual.

## Componentes Necessários
- Raspberry Pi Pico
- Matriz de LEDs WS2812B (5x5)
- 2 botões táteis
- Resistores de pull-up para os botões (caso necessário)
- Fios de conexão e protoboard (opcional)

## Conexão dos Componentes
- **Matriz WS2812B**:
  - Dados (DIN) -> GPIO 7 (OUT_PIN)
  - Alimentação -> 5V
  - GND -> GND
- **Botões**:
  - BTN_A -> GPIO 5
  - BTN_B -> GPIO 6
  - Ambos com resistores pull-up internos ativados
- **LED RGB**:
  - Vermelho -> GPIO 13
  - Verde -> GPIO 12
  - Azul -> GPIO 11

## Estrutura do Código
- **Definição de pinos**: Configuração dos GPIOs para matriz, botões e LED RGB.
- **Mapeamento da matriz**: Define a ordem física dos LEDs.
- **Padrões de exibição**: Arrays representando os números de 0 a 9.
- **Função `matrix_rgb`**: Converte valores de RGB para o formato aceito pela matriz WS2812B.
- **Função `draw_pio`**: Atualiza os LEDs com base no padrão atual.
- **Tratamento de interrupções**: Captura eventos dos botões e alterna os padrões exibidos.
- **Loop principal**: Atualiza a matriz e faz o LED RGB piscar.

## Como Compilar e Executar
1. Instale o SDK do Raspberry Pi Pico e configure o ambiente de desenvolvimento.
2. Compile o código com a toolchain do Pico.
3. Suba o binário para o Raspberry Pi Pico via USB.
4. Reinicie o Pico e visualize a matriz em funcionamento.

## Possíveis Melhorias
- Adicionar mais padrões além dos números.
- Implementar um menu para escolher efeitos de iluminação.
- Ajustar a intensidade dos LEDs dinamicamente.
- Sincronização com sensores externos para alteração automática de padrões.

## Licença
Este projeto é de código aberto e pode ser modificado livremente.

