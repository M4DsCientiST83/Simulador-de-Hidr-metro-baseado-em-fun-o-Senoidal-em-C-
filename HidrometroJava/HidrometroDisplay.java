import javax.swing.*;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.*;
import java.net.*;
import javax.imageio.ImageIO;

public class HidrometroDisplay extends JFrame {
    private BufferedImage fundo;
    private BufferedImage ponteiro;
    private double volume = 0.0;
    private double angulo = 0.0;
    private boolean connected = false;
    private static final double VOLUME_MAX = 10.0; // Configurable maximum volume
    private static final double ANGULO_MAX = 270.0; // Maximum rotation angle
    
    private class DisplayPanel extends JPanel 
    {
            @Override
protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D) g;

    g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
    g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);

    int imgW = getWidth();
    int imgH = getHeight();

    // --- FUNDO ---
    if (fundo != null) {
        g2d.drawImage(fundo, 0, 0, imgW, imgH, null);
    }

    // --- CENTROS DOS PONTEIROS ---
    int leftCenterX = (int) (imgW * 0.51);
    int leftCenterY = (int) (imgH * 0.64);
    int rightCenterX = (int) (imgW * 0.6182);
    int rightCenterY = (int) (imgH * 0.559);

    // --- CÁLCULO DOS DÍGITOS ---
    int totalLitros = (int) Math.round(volume * 1000); 
    int litrosUnidade = totalLitros % 10;       
    int litrosDezena  = (totalLitros / 10) % 10; 

    // --- ÂNGULOS ---
    double leftAngle  = Math.toRadians(-90 + (litrosDezena / 10.0) * 360);
    double rightAngle = Math.toRadians(-90 + (litrosUnidade / 10.0) * 360);

    // --- TAMANHO DOS PONTEIROS ---
    int pointerLength = Math.min(imgW, imgH) / 30; // comprimento do ponteiro
    int pointerWidth  = pointerLength / 2;        // largura proporcional fina
    AffineTransform at = new AffineTransform();

    if (ponteiro != null) {
    // Redimensiona o PNG do ponteiro
    BufferedImage scaledPointer = new BufferedImage(pointerWidth, pointerLength, BufferedImage.TYPE_INT_ARGB);
    Graphics2D gPtr = scaledPointer.createGraphics();
    gPtr.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
    gPtr.drawImage(ponteiro, 0, 0, pointerWidth, pointerLength, null);
    gPtr.dispose();

    // ---- acha automaticamente a BASE real do ponteiro no PNG (desconsidera transparência) ----
    int ax = pointerWidth / 2;   // fallback: meio
    int ay = pointerLength;      // fallback: base geométrica
    // procura a última linha não-transparente (potencial base)
    int yBase = -1;
    for (int y = pointerLength - 1; y >= 0 && yBase == -1; y--) {
        for (int x = 0; x < pointerWidth; x++) {
            int a = (scaledPointer.getRGB(x, y) >>> 24) & 0xff;
            if (a > 10) { yBase = y; break; }
        }
    }
    if (yBase >= 0) {
        // pega a largura útil nessa linha e centraliza
        int xMin = pointerWidth, xMax = -1;
        for (int x = 0; x < pointerWidth; x++) {
            int a = (scaledPointer.getRGB(x, yBase) >>> 24) & 0xff;
            if (a > 10) { xMin = Math.min(xMin, x); xMax = Math.max(xMax, x); }
        }
        if (xMax >= xMin) { ax = (xMin + xMax) / 2; ay = yBase; }
    }
    // -------------------------------------------------------------------------------------------

    // --- PONTEIRO ESQUERDO ---
    at.setToIdentity();
    at.translate(leftCenterX, leftCenterY); // leva o mundo ao pivot
    at.rotate(leftAngle);                   // gira no pivot
    at.translate(-ax, -ay);                 // ancora pela base real do PNG
    g2d.drawImage(scaledPointer, at, null);

    // --- PONTEIRO DIREITO ---
    at.setToIdentity();
    at.translate(rightCenterX, rightCenterY);
    at.rotate(rightAngle);
    at.translate(-ax, -ay);
    g2d.drawImage(scaledPointer, at, null);

} else {
    // fallback: linhas
    g2d.setColor(Color.RED);
    g2d.setStroke(new BasicStroke(pointerWidth));
    int leftEndX  = (int)(leftCenterX  + pointerLength * Math.cos(leftAngle));
    int leftEndY  = (int)(leftCenterY  + pointerLength * Math.sin(leftAngle));
    int rightEndX = (int)(rightCenterX + pointerLength * Math.cos(rightAngle));
    int rightEndY = (int)(rightCenterY + pointerLength * Math.sin(rightAngle));
    g2d.drawLine(leftCenterX,  leftCenterY,  leftEndX,  leftEndY);
    g2d.drawLine(rightCenterX, rightCenterY, rightEndX, rightEndY);
}


    // --- LCD METROS CÚBICOS REDIMENSIONÁVEL ---
    int lcdX = (int) (imgW * 0.41); // posição horizontal relativa
    int lcdY = (int) (imgH * 0.405); // posição vertical relativa

    // dimensões dos dígitos proporcionais ao tamanho da janela
    int digitWidth = imgW / 32;   // largura do dígito proporcional à largura da janela
    int digitHeight = imgH / 14;  // altura proporcional à altura da janela
    int spacing = digitWidth / 8; // espaço entre os dígitos

    // fonte proporcional
    g2d.setFont(new Font("Monospaced", Font.BOLD, digitHeight / 2));
    FontMetrics fm = g2d.getFontMetrics();

    // formata o volume em 6 dígitos
    String volumeStr = String.format("%08.0f", volume).replace(".", "");
    volumeStr = volumeStr.substring(volumeStr.length() - 6);

    // desenha cada dígito
    for (int i = 0; i < 6; i++) {
        int boxX = lcdX + i * (digitWidth + spacing);
        int boxY = lcdY - digitHeight + 5;

        // fundo do dígito
        g2d.setColor(Color.WHITE);
        g2d.fillRect(boxX, boxY, digitWidth, digitHeight);

        // borda do dígito
        g2d.setColor(Color.BLACK);
        g2d.drawRect(boxX, boxY, digitWidth, digitHeight);

        // dígito centralizado
        char digit = (i < volumeStr.length()) ? volumeStr.charAt(i) : '0';
        int digitX = boxX + (digitWidth - fm.stringWidth(String.valueOf(digit))) / 2;
        int digitY = boxY + ((digitHeight - fm.getHeight()) / 2) + fm.getAscent();

        g2d.drawString(String.valueOf(digit), digitX, digitY);
    }

    // --- STATUS ---
    g2d.setFont(new Font("Arial", Font.PLAIN, 12));
    String statusText = connected ? "Conectado" : "Desconectado";
    g2d.setColor(connected ? Color.GREEN : Color.RED);
    int statusWidth = g2d.getFontMetrics().stringWidth(statusText);
    g2d.drawString(statusText, imgW - statusWidth - 20, 30);
}
  
    }
    
    private DisplayPanel panel;
    private Thread connectionThread;
    
    public HidrometroDisplay() {
        setTitle("Display do Hidrômetro");
        setSize(500, 500);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null); // Center the window
        
        // Load images with error handling
        loadImages();
        
        panel = new DisplayPanel();
        add(panel);
        
        // Add a menu bar for controls
        setupMenuBar();
        
        // Start connection thread
        startConnectionThread();
    }
    
    private void loadImages() {
        try {
            // Try to load images from resources
            URL fundoUrl = getClass().getResource("/images/Hidrometro.png");
            URL ponteiroUrl = getClass().getResource("/images/Ponteiro.png");
            
            if (fundoUrl != null) {
                fundo = ImageIO.read(fundoUrl);
            } else {
                System.out.println("Warning: Background image not found at /images/Hidrometro.png");
            }
            
            if (ponteiroUrl != null) {
                ponteiro = ImageIO.read(ponteiroUrl);
            } else {
                System.out.println("Warning: Pointer image not found at /images/Ponteiro.png");
            }
        } catch (IOException e) {
            System.err.println("Error loading images: " + e.getMessage());
            e.printStackTrace();
        }
    }
    
    private void setupMenuBar() {
        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("Controles");
        
        JMenuItem reconnect = new JMenuItem("Reconectar");
        reconnect.addActionListener(e -> {
            if (connectionThread != null && connectionThread.isAlive()) {
                connectionThread.interrupt();
            }
            startConnectionThread();
        });
        
        JMenuItem exit = new JMenuItem("Sair");
        exit.addActionListener(e -> System.exit(0));
        
        menu.add(reconnect);
        menu.addSeparator();
        menu.add(exit);
        menuBar.add(menu);
        
        setJMenuBar(menuBar);
    }
    
    private void startConnectionThread() {
        connectionThread = new Thread(this::connectToServer);
        connectionThread.setDaemon(true);
        connectionThread.start();
    }
    
    private void connectToServer() {
        while (!Thread.currentThread().isInterrupted()) {
            try (Socket socket = new Socket("127.0.0.1", 5000);
                 BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {
                
                connected = true;
                SwingUtilities.invokeLater(panel::repaint);
                
                String line;
                while ((line = in.readLine()) != null && !Thread.currentThread().isInterrupted()) {
                    processReceivedData(line);
                }
                
            } catch (IOException e) {
                connected = false;
                SwingUtilities.invokeLater(panel::repaint);
                
                if (!Thread.currentThread().isInterrupted()) {
                    System.err.println("Connection lost: " + e.getMessage());
                    try {
                        Thread.sleep(5000); // Wait 5 seconds before retry
                    } catch (InterruptedException ie) {
                        Thread.currentThread().interrupt();
                        break;
                    }
                }
            }
        }
    }
    
    private void processReceivedData(String line) {
        try {
            // Parse data: tempo=0;volume=0.26
            String[] parts = line.split(";");
            boolean updated = false;
            
            for (String part : parts) {
                part = part.trim();
                if (part.startsWith("volume=")) {
                    double newVolume = Double.parseDouble(part.split("=")[1]);
                    if (newVolume != volume) {
                        volume = newVolume;
                        // No need to calculate angle anymore since we handle it in the display
                        updated = true;
                    }
                }
                // You can add more parameters here if needed
                // else if (part.startsWith("tempo=")) { ... }
            }
            
            if (updated) {
                SwingUtilities.invokeLater(panel::repaint);
            }
            
        } catch (NumberFormatException e) {
            System.err.println("Error parsing data: " + line + " - " + e.getMessage());
        }
    }
    
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            HidrometroDisplay frame = new HidrometroDisplay();
            frame.setVisible(true);
        });
    }
}