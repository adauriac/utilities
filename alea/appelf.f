C     pour me compiler : f77 appelf.f hasard.o -o appelf

      integer*4 R1279
      external R1279
      real*8 gauss,uniform
      external gauss
      iseed = 0
      call InitR1279(iseed)
      print *,"seed utilise ",iseed
      do  i=1,10
         y = uniform()
         x = gauss()
         print *,x,y
      enddo

      stop
      end
