#  Sistema de Controle de Aula — STM32

> Projeto Integrado | Sistemas Embarcados & Linguagens de Programação  
> ETE FMC — Série 34 DS | 2026

---

##  Sobre o Projeto

Sistema embarcado interativo desenvolvido para controle de aulas em laboratório, utilizando o microcontrolador **STM32**, um display **ST7735** e quatro botoeiras. O sistema gerencia presença de alunos, controla saídas da sala e gera relatórios ao final da aula.

---

##  Funcionalidades

### 1. Autenticação
- Ao iniciar, o sistema gera uma **senha numérica aleatória de 6 dígitos** (visível via inspeção de variável no debugger).
- O professor insere a senha usando as botoeiras (↑ ↓ para selecionar dígito, → para confirmar, ← para apagar).
- Após **3 tentativas incorretas**, o sistema é **bloqueado**.

### 2. Configuração Inicial
- O professor define a **quantidade máxima de alunos** da turma (1 a 99).
- Ajuste feito com os botões ↑ e ↓, confirmado com →.

### 3. Execução da Aula
| Botão | Ação |
|-------|------|
| ↑ (CIMA) | Registrar entrada de aluno (simula leitura facial com matrícula aleatória) |
| ← (ESQ) | Registrar saída do aluno (banheiro/água) |
| → (DIR) | Registrar retorno do aluno |
| ↑ + ↓ simultâneos | Encerrar aula |

**Restrições:**
- Não é possível registrar mais alunos do que o máximo configurado.
- No máximo **3 alunos** podem estar fora da sala simultaneamente.

### 4. Interface Visual (Display ST7735)
- Indicador `presentes/total` e **barra de progresso** colorida.
- Contador de alunos fora da sala (`FORA: X` ou `FORA: MAX`).
- Mensagens de feedback (aluno identificado, senha correta/incorreta, acesso bloqueado, etc.).

### 5. Encerramento e Relatório
Ao encerrar, o display exibe:
- Total de alunos registrados
- Total de saídas realizadas
- Tempo total de aula (hh:mm:ss)
- Tempo médio de permanência fora da sala (hh:mm:ss)

---

##  Hardware Utilizado

- Microcontrolador: **STM32** (clock via HSI, 8 MHz)
- Display: **ST7735** (comunicação SPI1)
- Entradas: **4 botoeiras** com pull-up e debounce por software (via TIM2)
- Timer: **TIM2** — interrupção a cada ~5 ms para debounce das botoeiras

---

##  Decisões de Implementação

### Máquina de estados via flags
O fluxo do sistema é controlado por variáveis de estado (`inicio_aula`, `estagio_senha`, `config_inicial`, `exe_aula`), permitindo que o loop principal roteie as ações corretamente sem uso de `switch/case` global.

### Debounce por software
O TIM2 dispara a cada ~5 ms. A função `DebounceSwitch` exige 20 leituras estáveis consecutivas antes de confirmar a mudança de estado de um botão, eliminando ruído mecânico.

### Geração de senha aleatória
A senha é gerada com `rand()` semeado por `HAL_GetTick()`, garantindo valores diferentes a cada execução. A faixa é de 100000 a 999999 (sempre 6 dígitos).

### Controle de tempo fora da sala
O sistema registra o `HAL_GetTick()` no momento de cada saída (até 3 simultâneas) e acumula o tempo ao retorno. O tempo médio é calculado ao encerrar.

### Bloqueio por tentativas
Após `MAX_TENT` (3) tentativas incorretas de senha, a flag `blocked` é ativada e o sistema congela — nenhuma entrada de botão é processada.

---

##  Vídeo de Apresentação

📺 [Assista no YouTube](https://www.youtube.com/seu-link-aqui)

---

##  Integrantes do Grupo

| Cauã Eduardo Silva e Thiago Pereira da Costa |
