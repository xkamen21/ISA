# ISA
### Předmět:
- ISA - Síťové aplikace a správa sítí

### Autor:
- Daniel Kamenický (xkamen21)

### Projekt:
#### Název:
- Discord bot.

#### Popis:
- Vytvořte program isabot, který bude působit jako bot na komunikační službě Discord.
- Bot se připojí na Discord server na kanál "#isa-bot" a bude reagovat na všechny zprávy zaslané ostatními uživateli.

#### Postavení a spuštění:
- Pro správné spuštění použijte jednu z variant:
1.  Přeložte zadáním příkazu 'make'.
Spusťte příkazem './isabot -t <bot_token>'

2.  Otevřete soubor Makefile a nahraďte token 'BOT_TOKEN' vašim tokenem
- a. Spusťte příkazem 'make run'
- b. Spusťte příkazem 'make run_verbose', spustí se s parametrem '--verbose'

- Pro vyčištění zdejte 'make clean'
- Pro nápovědu spusťte s parametrem -h: './isabot -h'

### Omezení:
- Projekt předpokladá, že bot je přídán na server.
- Daný server musí obsahovat text channel pojmenovaný 'isa-bot'.
- Pro vytvoření a připojení bota následujte návod: https://discordpy.readthedocs.io/en/latest/discord.html
- Na serveru @eva.fit.vutbr.cz "make run" a "make run_verbose" fungujou jen pri prvotnim prekladu a nefunguje zde make clean.
