      module a
      real*8,allocatable:: T(:)
      end module a
      
      use a
      implicit none
      character*1,allocatable:: comments(:)
      character*10 filename
      integer*8 ll
      integer*4 r
      integer*4,external::readfromfile,feli
!      r = readfromfile("essai.dat\0",comments,T,ll)
!      print *,r,ll
      print *,feli()
      print *,T(1),T(2),T(3)
      end


      integer*4 function feli()
      use a
      implicit none
      allocate(T(3))
      T(1) =1.1d0
      T(2) =2.2d0
      T(3) =3.3d0
      feli=0
      end function
