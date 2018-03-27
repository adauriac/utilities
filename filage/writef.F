      real*8 T(3)
      character*256 comments
      integer*8 ll
      integer*4 r
      integer*4,external::writeinfile
      T(1) = 1.1d0
      T(2) = 2.2d0
      T(3) = 3.2
      write(comments,*) "L= 6\n u = 0.34\nv =0.76\n"
      ll = 3*8;
      r = writeinfile("essai.dat\0",comments,T,ll)
      print *,r
      print *,T(1),T(2),T(3)
      end
