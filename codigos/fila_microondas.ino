#include <Keypad.h>
#include <cstring>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

// Cria o servidor web na porta 80
WebServer server(80);

// criando variável código html
const char* htmlPage = R"rawliteral(<!DOCTYPE html>
 <html lang="pt-BR">
  <head>
    <!-- Configurações básicas de encoding e viewport para boa exibição em dispositivos móveis -->
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />

    <!-- Título da aba do navegador -->
    <title>Esquenta com Brisa - Fila do Micro-ondas</title>

    <!-- Google Fonts:
         - Josefin Sans: usada no corpo do site (texto geral)
         - Bebas Neue: usada em títulos e botões para dar destaque -->
    <link rel="preconnect" href="https://fonts.googleapis.com" />
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
    <link
      href="https://fonts.googleapis.com/css2?family=Josefin+Sans:wght@400;600&family=Bebas+Neue&display=swap"
      rel="stylesheet"
    />

    <!-- Arquivo de estilos principal -->
    <link rel="stylesheet" href="main.css" />
  </head>
  <body>
    <!-- Cabeçalho fixo do site com a navegação principal -->
    <header class="header">
      <nav class="nav-container">
        <!-- Bloco com a “logo” (placeholder) e textos da marca -->
        <div class="logo-section">
          <!-- Espaço redondo para uma logomarca (aqui usamos um emoji como exemplo) -->
          <div class="logo-placeholder">🐕</div>

          <!-- Título e subtítulo da marca no cabeçalho -->
          <div class="brand-text">
            <!-- Título grande do topo (não confundir com o h2 do herói) -->
            <h1 class="brand-title">Esquenta com Brisa</h1>
            <p class="brand-subtitle">Fila do Micro-ondas - CI/UFPB</p>
          </div>
        </div>

        <!-- Menu de navegação (links de âncora para seções da mesma página) -->
        <ul class="nav-menu">
          <li><a href="#inicio">Início</a></li>
          <li><a href="#fila">Fila Atual</a></li>
          <li><a href="#como-funciona">Como Funciona</a></li>
        </ul>
      </nav>
    </header>

    <!-- Conteúdo principal da página -->
    <main class="main-container">
      <!-- Seção de destaque (“hero”) com um subtítulo e o formulário da fila -->
      <section class="hero-section" id="inicio">
        <div class="hero-content">
          <!-- Título da seção (visual) -->
          <h2 class="hero-title">🍽 Esquente sua comida sem pressa!</h2>

          <!-- Subtítulo explicativo -->
          <p class="hero-subtitle">
            Sistema de fila do micro-ondas do Centro de Informática
          </p>

          <!-- Cartão que envolve seu formulário original (IDs preservados) -->
          <div class="queue-card">
            <h3 class="card-title">Entrar na Fila</h3>

            <!-- Formulário original (mesma lógica do JS depende destes IDs) -->
            <form id="form" class="queue-form">
              <!-- Campo para o nome do usuário -->
              <!-- autocomplete com o valor "name" diz ao navegador que esse campo é para o NOME COMPLETO -->
              <input
                type="text"
                id="nome"
                placeholder="Seu nome"
                required
                class="input"
                autocomplete="name" 
                
              />

              <!-- Campo para o tempo em minutos (número inteiro) -->
              <input
                type="number"
                id="time"
                placeholder="Tempo (min)"
                min="1"
                step="1"
                required
                class="input"
              />

              <!-- Botão de enviar para adicionar à fila -->
              <button type="submit" class="btn join-btn">
                Adicionar à fila
              </button>
            </form>
            <!-- Banner privado com o código do usuário (não aparece na lista pública) -->
            <div id="my-code-banner" class="my-code-banner" hidden>
              <div class="my-code-line">
                <span class="label">Seu código:</span>
                <span id="my-code-value" class="code"></span>
              </div>
              <div class="my-code-actions">
                <button id="copy-code-btn" class="btn copy-btn" type="button">
                  Copiar código
                </button>
              </div>
              <p class="my-code-hint">
                Guarde este código. Quando chegar sua vez, você será notificado.
              </p>
            </div>
          </div>
        </div>
      </section>

      <!-- Seção que mostra a lista atual da fila -->
      <section class="current-queue" id="fila">
        <!-- Título da seção “Fila Atual” -->
        <h2 class="section-title">📋 Fila Atual</h2>

        <!-- Cartão que contém a ul da fila (IDs preservados) -->
        <div class="queue-list-card">
          <!-- A ul onde os itens da fila serão inseridos via JavaScript -->
          <ul id="list" class="queue-list"></ul>
        </div>
      </section>

      <!-- Seção explicando como o sistema funciona (apenas visual/texto) -->
      <section class="how-it-works" id="como-funciona">
        <!-- Título com cor de destaque -->
        <h2 class="section-title accent">🤔 Como Funciona?</h2>

        <!-- Passos ilustrativos em grid responsivo -->
        <div class="steps">
          <div class="step">
            <div class="step-number">1</div>
            <h3>Entre na Fila</h3>
            <p>Preencha seu nome e o tempo em minutos e envie.</p>
          </div>
          <div class="step">
            <div class="step-number">2</div>
            <h3>Acompanhe</h3>
            <p>Veja a lista e o tempo de espera acumulado.</p>
          </div>
          <div class="step">
            <div class="step-number">3</div>
            <h3>Sua Vez</h3>
            <p>Quando chegar sua vez, utilize o micro-ondas. Você será notificado com seu código.</p>
          </div>
      </section>
    </main>

 <!-- Rodapé com informações e placeholders de logos -->
 <footer class="footer">
  <div class="footer-content">
    <div class="footer-info">
      <p><strong>Esquenta com Brisa</strong> - Sistema de Fila do Micro-ondas</p>
      <p>Centro de Informática - Universidade Federal da Paraíba</p>
    </div>

    <!-- Espaço para logos (ex.: CI e UFPB) -->
    <div class="footer-logos">
      <div class="logo-ci-space">
        <img
          src="images/logo-ci.jpeg"
          class="footer-logo-img"
          loading="lazy"
        />
      </div>
      <div class="logo-ufpb-space">
        <img
          src="images/logo-ufpb.png"
          class="footer-logo-img"
          loading="lazy"
        />
      </div>
    </div>
  </div>
 </footer>
  <!-- Modal que aparece quando for a sua vez -->
    <div id="turn-modal" class="modal" hidden>
      <div class="modal-content">
        <h3 class="modal-title">É a sua vez! 🎉</h3>
        <p>Vá até o micro-ondas e insira o seu código.</p>
        <p class="modal-code">
          Seu código: <strong id="turn-code"></strong>
        </p>
        <p class="modal-instruction">
          Ao finalizar, pressione a tecla <strong>#</strong> no teclado do micro-ondas.
        </p>
        <div class="modal-actions">
          <button id="close-modal" class="btn join-btn" type="button">Ok</button>
        </div>
      </div>
    </div>
    <!-- Importa a lógica do site (arquivo JS). “defer” garante execução após o HTML estar parseado -->
    <script src="main.js" defer></script>
  </body>
 </html>)rawliteral";

const char* cssStyle  = R"rawliteral(/*
   Variáveis e base de estilo
   - Definimos cores, raios de borda e sombras para reutilizar*/
 :root {
  --bg: #e5e5e5;         /* Cor de fundo geral */
  --ink: #1c1c1c;        /* Cor do texto principal */
  --brand: #0a2b35;      /* Cor principal (cabeçalho/rodapé) */
  --brand-2: #255398;    /* Cor de destaque (botões/títulos) */
  --card: #ffffff;       /* Cor de fundo de cartões */
  --muted: #666666;      /* Texto secundário */
  --radius-xl: 24px;
  --radius-lg: 16px;
  --radius-md: 12px;
  --shadow-1: 0 4px 20px rgba(0, 0, 0, 0.06);
  --shadow-2: 0 3px 12px rgba(0, 0, 0, 0.08);
  --shadow-strong: 0 4px 20px rgba(10, 43, 53, 0.3);
 }

 /* Box sizing consistente e reset de margens/paddings básicos */
 *,
 *::before,
 *::after {
  box-sizing: border-box;
 }

 html, body {
  margin: 0;
  padding: 0;
 }

 /* Fonte padrão do corpo e cores básicas */
 body {
  font-family: "Josefin Sans", sans-serif; /* Carregada via Google Fonts */
  background: var(--bg);
  color: var(--ink);
  line-height: 1.6;
 }

 /* =========================================================
   Header / Navbar
   ========================================================= */
 .header {
  background: var(--brand);
  padding: 1.2rem 0;
  box-shadow: var(--shadow-strong);
 }

 .nav-container {
  max-width: 1200px;         /* Largura máxima centralizada */
  margin: 0 auto;
  padding: 0 2rem;
  display: flex;             /* Layout em linha */
  align-items: center;       /* Alinhamento vertical */
  justify-content: space-between; /* Espaçamento entre logo e menu */
 }

 .logo-section {
  display: flex;
  align-items: center;
  gap: 1rem;                 /* Espaço entre “logo” e textos */
 }

 /* Círculo que simula um espaço para a logo */
 .logo-placeholder {
  width: 56px;
  height: 56px;
  border-radius: 50%;
  background: #fff;
  border: 3px solid var(--brand-2);
  display: grid;
  place-items: center;       /* Centraliza o emoji */
  font-size: 1.4rem;
 }

 .brand-text {
  color: #fff;               /* Textos do topo são brancos */
 }

 /* Título da marca no header */
 .brand-title {
  margin: 0;
  font-family: "Bebas Neue", sans-serif; /* Fonte de destaque */
  letter-spacing: 0.5px;
  font-size: 1.8rem;
 }

 .brand-subtitle {
  margin: 0;
  font-size: 0.95rem;
  opacity: 0.9;
 }

 /* Menu de navegação (links) */
 .nav-menu {
  list-style: none;          /* Remove marcadores da lista */
  display: flex;
  gap: 1.2rem;
  margin: 0;
  padding: 0;
 }

 .nav-menu a {
  color: #fff;
  text-decoration: none;     /* Remove sublinhado */
  font-weight: 600;
  padding: 0.5rem 1rem;
  border-radius: 25px;
  transition: background-color 0.25s ease;
 }

 .nav-menu a:hover {
  background-color: rgba(255, 255, 255, 0.2);
 }

 /* =========================================================
   Layout principal do conteúdo
   ========================================================= */
 .main-container {
  max-width: 1200px;
  margin: 0 auto;
  padding: 2rem;             /* Espaçamento interno nas laterais */
 }

 /* =========================================================
   Seção Hero (destaque inicial)
   ========================================================= */
 .hero-section {
  background: var(--card);
  border-radius: var(--radius-xl);
  padding: 3rem 2rem;
  margin-bottom: 2rem;
  box-shadow: var(--shadow-1);
  text-align: center;
 }

 /* Título do hero (visual, não afeta a lógica JS) */
 .hero-title {
  margin: 0 0 0.75rem 0;
  font-size: 2rem;
  font-weight: 600;
  color: var(--ink);
  font-family: "Bebas Neue", sans-serif;
  letter-spacing: 0.5px;
 }

 /* Subtítulo do hero */
 .hero-subtitle {
  margin: 0 0 2rem 0;
  color: var(--muted);
 }

 /* Cartão que envolve o formulário */
 .queue-card {
  background: #fff;
  border-radius: var(--radius-lg);
  padding: 1.5rem;
  box-shadow: var(--shadow-2);
  max-width: 680px;
  margin: 0 auto;
 }

 /* Título do cartão do formulário */
 .card-title {
  margin: 0 0 1rem 0;
  font-size: 1.25rem;
  color: var(--brand);
  font-weight: 700;
 }
 /* =========================================================
   Formulário original (IDs preservados)
   ========================================================= */
 .queue-form {
  display: grid;
  grid-template-columns: 1fr 180px auto; /* Nome | Tempo | Botão */
  gap: 0.75rem;
 }

 /* Inputs padronizados */
 .input {
  padding: 0.8rem 0.9rem;
  border: 1.5px solid #e6e6e6;
  border-radius: 10px;
  font-size: 1rem;
  font-family: "Josefin Sans", sans-serif;
  outline: none;
  transition: border-color 0.2s ease, box-shadow 0.2s ease;
 }

 /* Efeito ao focar nos inputs */
 .input:focus {
  border-color: var(--brand-2);
  box-shadow: 0 0 0 3px rgba(46, 196, 241, 0.2);
 }

 /* Botões com destaque visual (classe compartilhada) */
 .btn {
  border: none;
  cursor: pointer;
  padding: 0.9rem 1.2rem;
  border-radius: var(--radius-md);
  font-weight: 700;
  transition: transform 0.1s ease, box-shadow 0.2s ease, background 0.2s ease;
  font-family: "Bebas Neue", sans-serif; /* Fonte de destaque nos botões */
 }

 /* Botão principal “Adicionar à fila” */
 .join-btn {
  background: var(--brand-2);
  color: #fff;
  box-shadow: 0 4px 12px rgba(46, 196, 241, 0.3);
 }

 .join-btn:hover {
  background:  #255398;
  transform: translateY(-1px);
  box-shadow: 0 6px 16px rgba(46, 196, 241, 0.35);
 }

 /* =========================================================
   Seção: Fila atual (lista gerada dinamicamente)
   ========================================================= */
 .current-queue {
  background: var(--card);
  border-radius: var(--radius-xl);
  padding: 2rem;
  box-shadow: var(--shadow-1);
  margin: 2rem 0;
 }

 /* Título de seção */
 .section-title {
  text-align: center;
  margin: 0 0 1.25rem 0;
  font-family: "Bebas Neue", sans-serif;
  letter-spacing: 0.5px;
  font-size: 1.6rem;
 }

 /* Variação de cor para título decorativo */
 .section-title.accent {
  color: var(--brand-2);
 }

 /* Cartão que envolve a lista UL */
 .queue-list-card {
  background: #fff;
  border-radius: var(--radius-lg);
  padding: 1rem;
  box-shadow: var(--shadow-2);
 }

 /* UL da fila (IDs preservados) */
 .queue-list {
  list-style: none;  /* Remove marcadores padrão */
  margin: 0;
  padding: 0;
 }

 /* Cada item da fila (LI) estilizado como “cardzinho” */
 .queue-list li {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0.9rem 1rem;
  border-bottom: 1px solid #f0f0f0;
  border-radius: 8px;
  margin-bottom: 0.4rem;
  background: #f9f9f9;
 }

 /* Destaque especial para o primeiro item (quem está usando agora) */
 .queue-list li:first-child {
  background: var(--brand-2);
  color: #fff;
  font-weight: bold;
 }

 /* =========================================================
   Seção “Como funciona”
   ========================================================= */
 .how-it-works {
  background: var(--card);
  border-radius: var(--radius-xl);
  padding: 2rem;
  box-shadow: var(--shadow-1);
  margin: 2rem 0;
 }

 /* Grade de passos responsiva */
 .steps {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
  gap: 1.2rem;
 }

 .step {
  text-align: center;
  padding: 1rem;
 }

 /* Bolinha numerada do passo */
 .step-number {
  width: 40px;
  height: 40px;
  background: var(--brand-2);
  color: #fff;
  border-radius: 50%;
  display: grid;
  place-items: center;
  font-weight: bold;
  margin: 0 auto 0.75rem auto;
 }

 /* =========================================================
   Rodapé
   ========================================================= */
 .footer {
  background: var(--brand);
  color: #fff;
  padding: 2rem 1rem;
  margin-top: 3rem;
 }

 .footer-content {
  max-width: 1200px;
  margin: 0 auto;
  display: flex;
  gap: 2rem;
  align-items: center;
  justify-content: space-between;
 }

 /* Espaços para logos no rodapé */
 .footer-logos {
  display: flex;
  align-items: center;
  gap: 1rem;
 }

 .logo-ci-space,
 .logo-ufpb-space {
  width: 68px;
  height: 68px;
  border-radius: 50%;
  background: #fff;
  border: 2px solid var(--brand-2);
  display: grid;
  place-items: center;
  box-shadow: 0 2px 8px rgba(46, 196, 241, 0.2);
 }
 
 .logo-ci-placeholder,
 .logo-ufpb-placeholder {
  color: var(--brand-2);
  font-size: 0.8rem;
  text-align: center;
  font-weight: 600;
  line-height: 1.2;
 }

 /* Imagens das logos no rodapé */
 .footer-logo-img {
  width: 100%;
  height: 100%;
  object-fit: contain; /* mantém proporção sem cortar */
  border-radius: 50%;  /* segue o formato circular do container */
  padding: 6px;        /* respiro interno */
  background: transparent;
 }
 /* =========================================================
   Responsividade (ajustes para telas menores)
   ========================================================= */
 @media (max-width: 900px) {
  /* Form vira 2 colunas: nome | tempo, botão pula para baixo */
  .queue-form {
    grid-template-columns: 1fr 1fr;
  }
  .join-btn {
    grid-column: 1 / -1; /* Botão ocupa toda a largura na linha de baixo */
  }
 }

 @media (max-width: 768px) {
  /* Navbar empilha conteúdo em telas menores */
  .nav-container {
    flex-direction: column;
    gap: 1rem;
    padding: 0 1rem;
  }
  .main-container {
    padding: 1rem;
  }
 }
 /* Banner privado do código do usuário */
 .my-code-banner {
  margin-top: 1rem;
  padding: 1rem;
  border-radius: var(--radius-md);
  background: #f7fbff;
  border: 1px solid #e0eefc;
  box-shadow: var(--shadow-2);
  text-align: left;
 }
 .my-code-line {
  display: flex;
  align-items: center;
  gap: 0.5rem;
 }
 .my-code-line .label {
  color: var(--muted);
  font-weight: 600;
 }
 .my-code-line .code {
  font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, "Liberation Mono", monospace;
  font-size: 1.1rem;
  letter-spacing: 1px;
  color: var(--brand-2);
 }
 .my-code-actions {
  margin-top: 0.5rem;
 }
 .copy-btn {
  background: #0e7d32;
  color: #fff;
 }
 .copy-btn:hover {
  background: #0c6b2a;
 }
 .my-code-hint {
  margin: 0.5rem 0 0 0;
  color: var(--muted);
  font-size: 0.9rem;
 }

 /* Modal (sua vez) */
 .modal {
  position: fixed;
  inset: 0;
  background: rgba(16, 24, 40, 0.5);
  display: grid;
  place-items: center;
  z-index: 1000;
 }
 .modal[hidden] {
  display: none;
 }
 .modal-content {
  background: #fff;
  border-radius: var(--radius-lg);
  padding: 1.5rem;
  max-width: 420px;
  width: calc(100% - 2rem);
  box-shadow: var(--shadow-strong);
  text-align: center;
 }
 .modal-title {
  margin: 0 0 0.5rem 0;
 }
 .modal-code {
  font-size: 1.1rem;
 }
 .modal-instruction {
  margin-top: 0.5rem;
  color: var(--muted);
 }
 .modal-instruction strong {
  color: var(--brand-2);
 }
 .modal-actions {
  margin-top: 1rem;
 })rawliteral";

//código java
const char* jsScript = R"rawliteral(// Pega as referências do DOM necessárias para manipular o formulário e a lista
const form = document.getElementById("form");   // <form id="form"> no HTML
 const list = document.getElementById("list");   // <ul id="list"> onde a fila será renderizada

// Estrutura em memória que guarda as pessoas na fila
let pessoas = [];
// Guarda o código da minha entrada (apenas no meu navegador) e se já avisei que é a minha vez
let meuCodigo = null;
let jaAviseiMinhaVez = false;

/**
 * Gera um código aleatório de 4 dígitos (entre 1000 e 9999).
 * Usado como um identificador simples para cada pessoa.
 */
function generateCode() {
  return Math.floor(1000 + Math.random() * 9000);
}

/**
 * Atualiza a interface da fila:
 * - Limpa a UL
 * - Recria os LIs com "Código | Nome | Tempo | Espera"
 * - Calcula e acumula o tempo de espera
 */

function atualizarFila() {
  fetch("/api/fila")
    .then(response => response.json())
    .then(data => {
      pessoas = data;
      list.innerHTML = "";

      let tempoAcumulado = 0;

      pessoas.forEach((pessoa, index) => {
        const tempoEspera = index === 0 ? 0 : tempoAcumulado;

        const li = document.createElement("li");
        const eu = meuCodigo && pessoa.code === meuCodigo ? " (você)" : "";
        li.textContent = `Nome: ${pessoa.nome}${eu} | Tempo: ${pessoa.time} min | Espera: ${tempoEspera} min`;

        list.appendChild(li);
        tempoAcumulado += pessoa.time;
      });

      // Verifica se é a vez do usuário
      if (
        pessoas.length > 0 &&
        meuCodigo !== null &&
        pessoas[0].code === meuCodigo &&
        !jaAviseiMinhaVez
      ) {
        jaAviseiMinhaVez = true;

        // Mostra o modal ou alerta
        alert(
          `É a sua vez! Vá ao micro-ondas e insira o seu código.\n\n` +
          `Seu código: ${meuCodigo}\n\n` +
          `Ao finalizar, pressione a tecla # no teclado do micro-ondas.`
        );
      }
    })
    .catch(error => {
      console.error("Erro ao buscar fila:", error);
    });
}

/**
 * Listener do formulário:
 * - Evita recarregar a página
 * - Lê os valores (nome e tempo)
 * - Valida os dados (nome não vazio; tempo > 0)
 * - Converte tempo para número (aceitando vírgula)
 * - Adiciona a pessoa à fila e re-renderiza
 * - Mostra o código apenas para o usuário (alert)
 */
form.addEventListener("submit", (e) => {
  e.preventDefault(); // Impede o comportamento padrão do form (evita refresh)

  // Lê e higieniza o nome (remove espaços extras no início/fim)
  const nome = document.getElementById("nome").value.trim();

  // Referência do input de tempo
  const timeInput = document.getElementById("time");

  // Converte o valor para número:
  // - Substitui vírgula por ponto (caso o usuário digite "2,5")
  // - Number(...) para obter um número real
  const parsedTime = Number(String(timeInput.value).replace(",", "."));

  // Regras de validação simples
  if (!nome) {
    alert("Informe um nome.");
    return;
  }
  if (!Number.isFinite(parsedTime) || parsedTime <= 0) {
    alert("Informe um tempo em minutos válido (maior que zero).");
    return;
  }

  // Gera um código para a pessoa (identificador simples)
  const code = generateCode();

  // Usa o tempo já convertido em número
  const time = parsedTime;

  // Adiciona a nova pessoa ao final da fila
  pessoas.push({ code, nome, time });

  // Envia os dados para a ESP
  fetch("/api/novo_usuario", {
   method: "POST",
   headers: {
    "Content-Type": "application/x-www-form-urlencoded",
  },
  body: `nome=${encodeURIComponent(nome)}&code=${code}`
 })
 .then(response => response.text())
 .then(data => console.log("Resposta da ESP:", data))
 .catch(error => console.error("Erro ao enviar dados:", error));
  
  // Guarda meu código localmente e reseta a flag de aviso
  meuCodigo = code;
  jaAviseiMinhaVez = false;

  // Mostra o código para o usuário (privado)
  alert(
    `Você entrou na fila!\n\n` +
    `Guarde o seu código: ${meuCodigo}\n\n` +
    `Quando for a sua vez, mostrarei um aviso aqui com seu código.\n` +
    `Ao finalizar, pressione a tecla # no teclado do micro-ondas.`
  );

  // Re-renderiza a fila na tela
  atualizarFila();
  setInterval(atualizarFila, 5000);
  
  // Limpa o formulário para a próxima entrada
  form.reset();
});)rawliteral";

// armazenando o nome e a senha da rede de wifi
const char* ssid = "ANAVI";
const char* password_wifi = "Ha@03111983";

// objeto lcd = placa, parâmetros: endereço em hexadecimal, linhas e colunas
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Declarando servo motor
Servo tranca;
const int SERVO_PIN = 23;

// declaração da quantidade de linhas e colunas
const byte ROWS = 4;
const byte COLS = 4;

// define as atribuições das teclas
const char TECLAS_MATRIZ[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// pins correspondentes as colunas
uint8_t colPins[COLS] = {26, 25, 33, 32};
// pins correspondentes as linhas
uint8_t rowPins[ROWS] = {13, 12, 14, 27};

// criando um objeto keypad que permite conseguir o objeto que está sendo digitado
Keypad keypad (makeKeymap(TECLAS_MATRIZ), rowPins, colPins, ROWS, COLS);

// criando o formato de usuarios
struct Usuario{
  String nome;
  String senha;
};

char senha_digitada[5];  // criando string para senha de 4 caracteres + '\0'
int senhaIndex = 0; // posição da srting


//tamanho máximo da fila, uma estimativa de 11h e 30min
const int TAM_MAX_FILA = 40;

// Fila de usuários
Usuario filaUsuarios[TAM_MAX_FILA];
int quantidade_pessoas = 0;
int inicio = 0;  // posição do primeiro da fila
int fim = 0;     // posição onde o próximo será inserido

void adicionar_pessoa(Usuario novoUsuario){
    if ((fim + 1) % TAM_MAX_FILA == inicio){
      Serial.println("A fila esta cheia! Retorne mais tarde");
    }
    else{
      filaUsuarios[fim] = novoUsuario;
      fim = (fim + 1) % TAM_MAX_FILA; // se chegar ao fim volta para o início
      quantidade_pessoas += 1;
    }
};

// contagem do tempo até digitação da senha
unsigned long tempo_inicio_digitacao = 0;
const unsigned long TEMPO_LIMITE = 90000; // 1 minuto e meio
bool tempo_contando = false;

void setup() {
 Serial.begin(115200);

 // inicialização da comunicação wifi
 WiFi.begin(ssid, password_wifi);

 // Servir HTML
 server.on("/", []() {
 server.send(200, "text/html", htmlPage);
  });

 // servidor css
 server.on("/main.css", []() {
 server.send(200, "text/css", cssStyle);
 });

 //servidor java
 server.on("/main.js", []() {
 server.send(200, "application/javascript", jsScript);
 });
  
 server.on("/api/novo_usuario", HTTP_POST, []() {
  String nome = server.arg("nome");
  String senha = server.arg("code");

  // Checa se há espaço na fila
  if (quantidade_pessoas >= TAM_MAX_FILA) {
    server.send(503, "text/plain", "Fila cheia!");
    return;
  }

  // Inserir no fim da fila circular
  filaUsuarios[fim].nome = nome;
  filaUsuarios[fim].senha = senha;
  fim = (fim + 1) % TAM_MAX_FILA;
  quantidade_pessoas++;

  server.send(200, "text/plain", "Usuário recebido com sucesso.");
 });

 server.on("/api/fila", HTTP_GET, []() {
  String json = "[";
  for (int i = 0; i < quantidade_pessoas; i++) {
    int idx = (inicio + i) % TAM_MAX_FILA;

    json += "{";
    json += "\"nome\":\"" + filaUsuarios[idx].nome + "\",";
    json += "\"code\":\"" + filaUsuarios[idx].senha + "\",";
    json += "\"time\":5";  // Valor fixo de tempo (se quiser usar isso no JS)
    json += "}";

    if (i < quantidade_pessoas - 1) json += ",";
  }
  json += "]";

  server.send(200, "application/json", json);
 });

 //iniciando servidor
  server.begin();

 //placa lcd
 lcd.init();
 lcd.backlight();

 // confirmação de conexão com wifi
 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  lcd.print("Tentando conexao");
  delay(1000);
}
 
 lcd.setCursor(0, 1);
 lcd.print("Wifi conectado");
 delay(2000);
 lcd.clear();

 //confirmando iniciação
 lcd.println("Servidor iniciado.");
 delay(1000);
 lcd.clear();
 lcd.println(WiFi.localIP());
 delay(2000);
 lcd.clear();

 // servo motor
 tranca.attach(SERVO_PIN); //declarando a posição do servo
 tranca.write(0); // começa trancado

}


void loop() {
// funcionamento servidor web
server.handleClient();

if(quantidade_pessoas != 0){ 
 char key = keypad.getKey();

 // Mostrar a mensagem só se for a primeira tecla e iniciar a contagem do timer
 if (senhaIndex == 0 && !tempo_contando) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite sua senha");
  lcd.setCursor(0, 1);
  lcd.print(filaUsuarios[inicio].nome);
  lcd.print("!");
  
  tempo_inicio_digitacao = millis();
  tempo_contando = true;
 }

 if(key){
  if (senhaIndex < 4) {
   senha_digitada[senhaIndex] = key;
   senhaIndex++;
   senha_digitada[senhaIndex] = '\0';  // Atualiza o terminador
    }
  
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print(filaUsuarios[inicio].nome);
   lcd.setCursor(0,1);
   lcd.print("Senha: ");
   lcd.print(senha_digitada);

  // Quando senha completa for digitada (4 caracteres)
  if (senhaIndex == 4) {
    delay(2000); // Espera 2 segundos antes de limpar
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha final:");
    lcd.setCursor(0, 1);
    lcd.print(senha_digitada);
    delay(2000);
    lcd.clear();
    
    //conferindo se a senha digitada é igual a definida
    if (String(senha_digitada) == filaUsuarios[inicio].senha){
      lcd.print("Acesso liberado!");
      tranca.write(180); // Gira para 180°
      delay(1000);         // Espera 1 segundo na posição
      tranca.write(0);
       
      lcd.clear();
      lcd.print("Pressione #");
      lcd.setCursor(0, 1);
      lcd.print("para finalizar");

      while (true) {
       char end = keypad.getKey();
       if (end == '#') {
       lcd.clear();
       lcd.print("Finalizando...");
       tranca.write(180); 
       delay(1000);
       tranca.write(0); // volta pra posição inicial
       inicio = (inicio + 1) % TAM_MAX_FILA;
       quantidade_pessoas--;
       senhaIndex = 0;
       senha_digitada[0] = '\0';
       tempo_contando = false;
       break; // Sai do while e volta ao início do loop()
   }
 }
}    
    else {
     lcd.print("Acesso negado!");
    }

   // Limpa buffer e recomeça
   senhaIndex = 0;
   senha_digitada[0] = '\0';
   tempo_contando = false;   
   delay(2000);
   lcd.clear();
    }
  }

 if (tempo_contando && (millis() - tempo_inicio_digitacao >= TEMPO_LIMITE)) {
  lcd.clear();
  lcd.print("Tempo esgotado!");
  delay(2000);
  lcd.clear();

  inicio = (inicio + 1) % TAM_MAX_FILA;
  quantidade_pessoas--;

  senhaIndex = 0;
  senha_digitada[0] = '\0';
  tempo_contando = false;  // Resetar timer
  return;  // Volta ao início do loop()
 } 

}

else{
  lcd.setCursor(0,0);
  lcd.print("Fila vazia!");
}
}