while(1)
	{
		sa = accept(s,0,0);		/*Block for connection request*/
		if (sa < 0) perror("accept failed");

		read(sa,buf,BUF_SIZE);	/*Read file name from socket*/

		/*Get and return the file.*/
		fd = open(buf,O_RDONLY);	/*Open fail to send bakc*/
		if (fd < 0) perror("open failed");

		while(1)
		{
			bytes = read(fd,buf,BUF_SIZE);
			if (bytes <= 0) break;
			write(sa, "HTTP/1.1 200 OK\n", 16);
			write(sa,"Hello world\n",12);
		}
		close(fd);
		close(sa);
	}