// 4. Imagine you have a 20GB file with one string per line. Explain
// how you would sort the file, with 1GB memory

// 1. Divide the file into 20 chunks, the size of each chunk is 1GB
// 2. sort each chunk with qsort (due to fast and inplace)
// 3. Merge the chunks
// 		a. Read all chunk up to 25MB each
//      b. Merge the chunk into 500MB buffer
//		c. Write the buffer to the disk when it is full
//		d. Read further 25MB from each chunk if all data is merged.
