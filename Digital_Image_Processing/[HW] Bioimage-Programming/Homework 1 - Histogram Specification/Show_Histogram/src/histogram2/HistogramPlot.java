package histogram2;
import ij.ImagePlus;
import ij.gui.NewImage;
import ij.process.ImageProcessor;

/* W. Burger, M. J. Burge: "Digitale Bildverarbeitung" 
 * © Springer-Verlag, 2005
 * www.imagingbook.com
*/

public class HistogramPlot {
	static final int BACKGROUND = 255;
	//String title = "Histogram";
    int width =  256;
    int height = 128;
    int base = height-1;
    int paintValue = 0;
	ImagePlus hist_img;
	ImageProcessor ip;
	int[] H = new int[256];
	
	public HistogramPlot(double[] nH, String title){
		createHistogramImage(title);
		// nH mus be a normalized histogram of length 256
		for (int i=0; i<nH.length; i++) {
			H[i] = (int) Math.round(height*nH[i]);
		}
		draw();
		//show();
	}
	
	public HistogramPlot(PiecewiseLinearCdf cdf, String title){
		createHistogramImage(title);
		// nH mus be a normalized histogram of length 256
		for (int i=0; i<256; i++) {
			H[i] = (int) Math.round(height*cdf.getCdf(i));
		}
		draw();
		//show();
	}
	
	void createHistogramImage(String title) {
		if (title == null)
			title = "Histogram Plot";
		hist_img  = NewImage.createByteImage(title,width,height,1,0);
		ip = hist_img.getProcessor();
        ip.setValue(BACKGROUND);
        ip.fill();
	}
	
	void draw() {
		ip.setValue(0);
		ip.drawLine(0,base,width-1,base);
		ip.setValue(paintValue);
		int u = 0;
		for (int i=0; i<H.length; i++) {
			int k = H[i];
			if (k>0){
			ip.drawLine(u,base-1,u,base-k);
			//ip.drawLine(u+1,base-1,u+1,base-k);
			}
			u = u+1;
		}
	}
	
	void update() {
		hist_img.updateAndDraw();
	}
	
	public void show() {
		hist_img.show();
        update();
	}
	
	void makeRamp() {
		for (int i=0; i<H.length; i++) {
			H[i] = i;
		}
	}
	
	void makeRandom() {
		for (int i=0; i<H.length; i++) {
			H[i] = (int)(Math.random()*height);
		}
	}
	
    //----- static methods ----------------------
	
    public static void showHistogram(ImageProcessor ip, String title) {
		int[] Ha = ip.getHistogram();
		double[] nH = Util.normalizeHistogram(Ha);
		HistogramPlot hp = new HistogramPlot(nH, title);
		hp.show();
	}

	public static void showCumHistogram(ImageProcessor ip, String title) {
		int[] Ha = ip.getHistogram();
		//double[] nH = HistSpec.Pdf(Ha);
		double[] cH = Util.Cdf(Ha);
		HistogramPlot hp = new HistogramPlot(cH, title);
		hp.show();
	}

}
