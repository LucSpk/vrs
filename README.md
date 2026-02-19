17-02-2026: Hoje eu decidi voltar a estudar programação em C, já venho hà algumas semanas estudando programação de maneira mais experimental, fazendo pequenos commits por dia, nada muito elaborado, mas tentando pisar em terras que nunca pisei, fiz algumas coisas em assembly, simplesmente porque me anima aprender coisas que se relacionam com o hardware, e como estamos em 2026, o chatgpt vem me ajudando bastante a fazer esses experimentos. Também iniciei um projeto em java, para fazer um pouco mais do mesmo, para os dias em que não estou com cabeça, ou tempo para fazer algo mais fora da casinha. Já vem um tempo que quero voltar a botar a mão em C, já tem uns bons anos, desde a graduação e mais ainda, desde o técnico hehe. Nas últimas, sei lá, duas horas, venho pensando em algo que fosse realmente interessante de fazer, que não fosse só algo que dá para fazer em qualquer outra linguagem de mais alto nível cheia de abstrações. Acho que desde o início eu queria fazer a minha própria versão de um gerenciador de versão, algo como o git, estava procurando algumas outras possibilidades, mas nada chamou minha atenção, são sempre as mesmas coisas (calculadora, sistema de banco, quiz, sistema de restaurantes, etc), então decidi continuar com o controle de versão, na verdade eu nem tenho um nome ainda, esse repo na minha estação ainda está como "sistema de controle de vercao"... Sim, eu escrevi o "versão" errado, agora sei que preciso corrigir. Desde já sei que vou precisar de muita ajuda do Google, Reddit e de algumas IA´s para fazer algo realmente decente, até porque eu não sou nada brilhante para essas coisas e estou bem longe de ser Linus Torvalds, mas pretendo fazer algo que pelo menos não o envergonhe, vou me debruçar sobre isso sempre que possível, espero chegar longe o suficiente para que isso pelo menos se pareça com o que se propõe. Então é isso, vou corrigir o bendito nome do diretório e dar o pontapé inicial. Sempre que tiver algo de interessante para falar, vou tentar voltar aqui. 

Ps. Sejam gentis.



Att. Lucas Alves



17-02-2026: Andei lendo alguns artigos e acho que primeiro preciso eleger alguns problemas iniciais a serem resolvidos, o que devem ser como as features da coisa toda.

1. Como diacho vou "iniciar um repositório", começar a acompanhar as mudanças dos arquivos que estão dentro do diretório em tempo real. 
2. Como salvar versões dos arquivos conforme tudo for sendo modificado. 
3. Esse item meio que anda como anterior, meio que preciso identificar cada "versão" dos arquivos, para que eu possa voltar a elas depois, ou comparar as mudanças, ou seja lá o que for possível fazer com isso, e além disso, adicionar macros de tudo, quem fez, quando fez, etc.
4. Conseguir comparar as versões de cada arquivo.
5. Manter um histórico da coisa toda.
6. Garantir que isso tudo funcione de forma colaborativa. 

17-02-2026: Acabei de me deparar com o setimo problema.
7. Fazer tudo isso funfar através de linha de comando.

18-02-2026: Bem como pode ver, depois de algumas pesquisas, consegui uma forma de encontrar os arquivos do diretório. Agora estou tentandodescobrir como extender isso para os diretórios dentro do diretório principal. 

19-02-2026: Com a ajuda da lib sys/stat.h eu consegui ver o que é e o que não é diretório através do S_ISDIR(info.st_mode). Mas hoje estou com encalhado com um problema que esta realmente me fazendo pesquisar bastante. É bem simples de entender, Mas aparentemente nada fácil de se implementar, heheh, Atualmente estou simplesmente jogando no console os caminhos dos arquivos, mas isso de nada adianta, preciso ter esses caminhos gravados em algum lugar para que eu possa usar sempre que precisar e transoformar essa função em algo versátil. Seguindo essa linha, tenho que por tudo isso em uma estrutura de dados o que em C é um belo desafio, ja que não sei nem quantos arquivos vou ter no total, nem o tamanho do file path de cada um. Em algumas pesquisas eu vi que o normal é usar um malloc, mas ta dificil para entender como isso funciona, e como usar isso.