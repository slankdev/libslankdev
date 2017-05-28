

CXXFLAGS = -std=c++11
LDFLAGS  = -lX11


all: main




num_scrn: num_screen.cc
	g++ $(CXXFLAGS) $< -lXrandr $(LDFLAGS)


get_mouse: get_mouse.cc
	g++ $(CXXFLAGS) $< -lXrandr $(LDFLAGS)

create_win: create_win.cc
	g++ $(CXXFLAGS) $< -lXrandr $(LDFLAGS)

main: main.cc
	g++ $(CXXFLAGS) $< -lXrandr $(LDFLAGS)

run: all
	./a.out

clean:
	rm -rf a.out






xrandr-multi:
	xrandr --output DP2 --right-of eDP1

xrandr-mirror:
	xrandr --output DP2 --same-as eDP1
