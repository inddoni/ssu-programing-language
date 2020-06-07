package fileSearch20180445;

import java.io.File;

public class FileSearch implements Runnable{
    //public List<File> fileList = new ArrayList<File>();
    //public List<String> fileList = new ArrayList<String>();
    public File mfile;

    public boolean finished;
    public void FileReaderRunnable(File file) {
        this.mfile = file;
    }

    @Override
    public void run() {
        getfiles(mfile);
    }

    public void setFlag(boolean value) {
        finished = value;
    }

    public void getfiles(File file) {
        System.out.println("EXecuting...: "+Thread.currentThread().getName()+file.getAbsolutePath());
        File[] listFiles = file.listFiles();
        if (listFiles != null && listFiles.length > 0) {
            for (File file2 : listFiles) {
                //if(!fileList.contains(file2.getAbsoluteFile())){
                //    fileList.add(file2.getAbsoluteFile());
                //}
                //getfiles(file2);
            }
        }
    }

//    public List<File> getFiles(){
//        return fileList ;
//    }

}
