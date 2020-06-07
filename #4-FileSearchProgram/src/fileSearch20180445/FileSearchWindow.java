package fileSearch20180445;
import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import javax.swing.*;

import static javax.swing.JOptionPane.showMessageDialog;


public class FileSearchWindow extends JFrame{
    public JTextField fPath;
    public JTextField fName;
    //생성
    public FileSearchWindow(){


        JPanel panel1 = new JPanel();
        JPanel panel2 = new JPanel();
        JPanel panelBtn = new JPanel();

        Container fsearchFrame = this.getContentPane();
        fsearchFrame.setLayout(new FlowLayout());

        fPath = new JTextField(20);
        panel1.add(new JLabel("FilePath : "));
        panel1.add(fPath);
        fName = new JTextField(20);
        panel2.add(new JLabel("Keyword : "));
        panel2.add(fName);

        JButton btnSearch = new JButton("Search");
        JButton btnExit = new JButton("Exit");
        btnSearch.addActionListener(SearchBtn);
        btnExit.addActionListener(ExitBtn);
        panelBtn.add(btnSearch);
        panelBtn.add(btnExit);



        fsearchFrame.add(panel1);
        fsearchFrame.add(panel2);
        fsearchFrame.add(panelBtn);

        this.setTitle("File Search - 20180445");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(400,300);
        this.setVisible(true);

    }


    ActionListener SearchBtn = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            String path = fPath.getText();
            String fileName = fName.getText();
            try {
                if(path.length() == 0 || fileName.length() == 0){
                    throw new MyException("경로를 입력하셔야 합니다.");
                }
                else {
                    System.out.println(path);
                    Searching s1 = new Searching();
                }
            }catch (MyException me){
                showMessageDialog(null, "경로를 입력하셔야 합니다.");
            }

        }
    };

    ActionListener ExitBtn = new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            System.exit(0);
        }
    };


    public static void main(String[] args) {
        ExecutorService executor = Executors.newFixedThreadPool(30);
        File file = new File("C:\\Temp");
        FileReaderRunnable r = new FileReaderRunnable(file);
        long startTime = System.nanoTime();
        executor.execute((Runnable) r);
        executor.shutdown();
        // Wait until all threads are finish
        while (!executor.isTerminated()) {
        }
        long endTime = System.nanoTime();
        System.out.println("\nFinished all threads");
        System.out.println("main thread exited");
        //List<File> files = r.getFiles();
        //System.out.println("Total Files size: "+files.size());
        long duration = (endTime - startTime);
        System.out.println("Duration: "+duration/1000000 );
        FileSearchWindow fsw = new FileSearchWindow();
    }
}