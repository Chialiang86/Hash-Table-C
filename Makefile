CC = gcc
GP = gnuplot

# for c code and execution files
NLP = nlp_v
GENERATE = generate
SPEED = speed

all: $(NLP) $(SPEED) $(GENERATE)

%.o: %.c
	$(CC) -c $^

nlp: nlp_v.o
	$(CC) -o $(NLP) $<

speed: speed.o
	$(CC) -o $(SPEED) $<

generate: generate.o
	$(CC) -o $(GENERATE) $<

# for data digure
DATDIR = .dat/
FIGDIR = .fig/

PDIST = plot_distribution.gp
PHASH = plot_hash.gp 
PSPEED = plot_speed.gp
PSCAL = plot_scalability.gp 

plot: $(GENERATE)
	./$(GENERATE) 10 256 20
	$(GP) $(PHASH)
	$(GP) $(PSCAL)

format:
	clang-format -i *.[ch]

clean:
	rm *.o $(NLP) $(SPEED) $(GENERATE)