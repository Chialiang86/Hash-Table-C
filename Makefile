CC = gcc
GP = gnuplot

# for c code and execution files
NLP = nlp_v
GENERATE = generate
SPEED = speed

all: $(NLP) $(SPEED) $(GENERATE)
nlp: $(NLP)
speed: $(SPEED)
generate: $(GENERATE)

%.o: %.c
	$(CC) -c $^

$(NLP): nlp_v.o
	$(CC) -o $@ $<

$(SPEED): speed.o
	$(CC) -o $@ $<

$(GENERATE): generate.o
	$(CC) -o $@ $<

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

clean:
	rm *.o $(NLP) $(SPEED) $(GENERATE)