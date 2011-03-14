        program spektro
        implicit none
        double precision :: average(740,1), dummy, single, rgb(121360,1)
        double precision :: x, y
        integer :: i, j, narg, n_samples, y_int
        character(LEN=50) :: filename, outfile, infile
        character(LEN=3) :: char_n_samples, char_i

        
        narg = iargc()

        !Zero averge
        average=0.0d0

        !Fix this later
        n_samples=3

        !Lets take some pictures!
        do i = 1, n_samples
        write (char_i,'(i3)') i
        outfile= TRIM(ADJUSTL(char_i //'.png'))
        call system('gst-launch-0.10 v4l2src ! ffmpegcolorspace &
     &  ! pngenc ! filesink location=' // outfile)
        call SLEEP(1)
        end do

        !Convert the picture to text
        do i = 1, n_samples
        write (char_i,'(i3)') i
        infile= TRIM(ADJUSTL(char_i //'.png'))
        outfile= TRIM(ADJUSTL(char_i //'.txt'))
        call system('convert -crop 740x164+235+370 -colorspace Gray ' &
     &  // infile // ' ' // outfile)
        call system('sed -i "s/,/ /g" ' // outfile )
        call system('sed -i "s/:/ /g" ' // outfile )
        call system('sed -i "s/(/ /g" ' // outfile )
        end do
     
        !Lets try loading this into an array 
        do i = 1, n_samples

        !Read the text file into an array
        infile= TRIM(ADJUSTL(char_i // '.txt'))
        open (14,file=infile,status="OLD")
        read (14,*) ! First line is crap
        do j = 1, 121360
        read (14,*) dummy, dummy, rgb(j,1)
        end do !j
        close(14)

        !Get the value we need from the array
        do j = 1, 740
        !What Y value do we need?
        y = -( 0.162189d0*DBLE(j) - 148.613d0 )
        y_int = IDINT(y)
        average(j,1) = average(j,1) + rgb(y_int*740+j,1)
        !Crazy numbers seemed to come from average not being initialized
        !to Zero...
        !if ( ISNAN(rgb(y_int*740+j,1)) ) then
        !write (*,*) "NaN line " , y_int*740+j, "file ", infile
        !end if
        !if ( rgb(y_int*740+j,1) > 100) then
        !write (*,*) ">100 line " , y_int*740+j, "file ", infile
        end if



        end do !j

        end do !i

        ! Lets put everything back into a file
        open(21,file="average.dat",STATUS="UNKNOWN")
        do i = 1, 740
        write (21,*) i, average(i,1)/n_samples
        end do
        close(21)

        end program
