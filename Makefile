CC = gcc
GP = gnuplot

# for c code and execution files
MAIN = main
GENERATE = generate
SPEED = speed

all: $(MAIN) $(SPEED) $(GENERATE)
main: $(MAIN)
speed: $(SPEED)
generate: $(GENERATE)

%.o: %.c
	$(CC) -c $^

$(MAIN): main.o
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
PSCAL = plot_hash.gp 
PSPEED = plot_speed.gp

plot: $(GENERATE)
	./$(GENERATE) 10 256 20
	$(GP) $(PDIST)
	$(GP) $(PHASH)
	$(GP) $(PSCAL)

psca:

clean:
	rm *.o $(MAIN) $(SPEED) $(GENERATE)