USRSRC = Osi.cpp
TARGETLIB = Osi.a

OBJS = $(USRSRC:.cpp=.o)

SHARED = -shared -o
FPIC = -fPIC -o

.PHONY:all
all:$(TARGETLIB)

$(TARGETLIB):$(OBJS)
	$(CXX) $< $(SHARED) $(FPIC) $@

$(OBJS):$(USRSRC)
	$(CXX) -c $< $(FPIC) $@

SOURCE_D = $(USRSRC:.cpp=.d)
-include $(SOURCE_D)
%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$;\
	@sed -e;rm -fr $@;\
	@sed 's,\($*\).o[ :],\1.o $@ :,g' <$@.$$$$ > $@;\
	rm -fr $@.$$$$;
clean:
	-rm -f $(TARGETLIB)
	-rm -f $(OBJS)

#.PHONY:Osi.a
#Osi.a:Osi.o
#	g++ Osi.o -shared -o -fPIC -o Osi.a
#Osi.o:Osi.cpp Osi.h	
#	g++ -c Osi.cpp -fPIC -o Osi.o	
#
#clean:
#	-rm -fr Osi.o
#	-rm -fr Osi.a
#	-rm -f $(TARGETLIB)