import ij.IJ;
import ij.ImagePlus;
import ij.plugin.filter.PlugInFilter;
import ij.process.ImageProcessor;
import histogram2.*;
import ij.gui.*;
import ij.WindowManager;

public class Show_Histogram implements PlugInFilter { 
	
	String title;
	boolean runHistEqualization = false;
	boolean runPiecewise = false;
	boolean runGaussian = false;
	boolean runMatchImage = false;

	ImagePlus imB;
	
	public int setup(String arg0, ImagePlus im) {
		title = im.getTitle();
		return DOES_8G+DOES_STACKS;
	}
	

	public void run(ImageProcessor ip) {
            
        
		if (!showDialog())
			return;
		// Show the histogram and cum. histogram of original image
		//HistogramPlot.showHistogram(ip, "His of " + title);
        //HistogramPlot.showCumHistogram(ip, "Cum. Hist of " + title);
        if (runMatchImage)
        {
			Match_To_Image_Histogram(ip,title);
        }	

        if (runHistEqualization) 
	    	histEqualization(ip,title);
	    if (runPiecewise)
	    	Match_To_Piecewise_Linear_Histogram(ip,title);
	    if (runGaussian)
	    	Match_To_Gaussian_Histogram(ip,title);
	}


	/*
	 * 4.5 Linear Histogram Equalization 
	 */
	public static void histEqualization(ImageProcessor ip, String title){
		int M = ip.getWidth();
		int N = ip.getHeight();
		int K = 256; // number of intensity values

		// Compute the cumulatie histogram
		int [] H = ip.getHistogram();
		for (int j = 1; j < H.length; j++) {
			 H[j] = H[j-1] +H[j];
		}

		// Equalize the image
		for (int v = 0 ; v < N; v ++) {
        	for (int u = 0; u < M; u ++) {
        		int a = ip.get(u,v);
        		int b = H[a] * (K-1) / (M*N); // equation
        		ip.set(u,v,b);
        	}
        }
        //HistogramPlot.showHistogram(ip, "His after Equalization of " + title);
        //HistogramPlot.showCumHistogram(ip, "Cum. His after Equalization of " + title);
	}

	/*
	 * Match To Piecewise Linear Histogram
	 */
	public static void Match_To_Piecewise_Linear_Histogram(ImageProcessor ip, String title) {
		// get histogram of original image
		int[] hA = ip.getHistogram();
		// -------------------------
		int[] ik = {28, 75, 150, 210};
		double[] Pk = {.05, .25, .75, .95};
		PiecewiseLinearCdf pLCdf =
			new PiecewiseLinearCdf(256, ik, Pk);
		// -------------------------
		
		double[] nhB = pLCdf.getPdf();
		nhB = Util.normalizeHistogram(nhB);
		//(new HistogramPlot(nhB, "Piecewise Linear")).show();
		//(new HistogramPlot(pLCdf, "Piecewise Linear Cumulative")).show();
		
		HistogramMatcher m = new HistogramMatcher();
		int[] F = m.matchHistograms(hA, pLCdf);
		
		for (int i=0; i<F.length; i++) {
			IJ.write(i + " -> " + F[i]);
		}
		
		ip.applyTable(F);
		//HistogramPlot.showHistogram(ip, "His A (mod)");
		//HistogramPlot.showCumHistogram(ip, "Cum His A (mod)");
	}

	/*
	 * Match To Gaussian Histogram
	 */
	public static void Match_To_Gaussian_Histogram(ImageProcessor ip, String title) {
		// get histogram
		int[] hA = ip.getHistogram();
		int[] hB = Util.makeGaussianHistogram(128,50);
		
		double[] nhB = Util.normalizeHistogram(hB);
		(new HistogramPlot(nhB, "Gauss")).show();
		
		double[] chB = Util.Cdf(hB);
    	//HistogramPlot hp = new HistogramPlot(chB, "Gauss cumulative");
		//hp.show();
		
		HistogramMatcher m = new HistogramMatcher();
		int[] F = m.matchHistograms(hA, hB);
		
		for (int i=0; i<F.length; i++) {
			IJ.log(i + " -> " + F[i]);
		}
		
		ip.applyTable(F);
		// HistogramPlot.showHistogram(ip, "Histogram A (mod)");
		// HistogramPlot.showCumHistogram(ip, "Cumulative Histogram A (mod)");
	}

	/*
	 * Match To Image Histogram
	 */
	public void Match_To_Image_Histogram(ImageProcessor ip, String title) 
	{
		/* Match to image histogram */
	    ImageProcessor ipB = imB.getProcessor();
		//HistogramPlot.showHistogram(ip, "Histogram A");
		//HistogramPlot.showHistogram(ipB, "Histogram B");
		//HistogramPlot.showCumHistogram(ip, "Cumulative Histogram A");
		//HistogramPlot.showCumHistogram(ipB, "Cumulative Histogram B");
		// get histograms of both images
		int[] hA = ip.getHistogram();
		int[] hB = ipB.getHistogram();
		
		HistogramMatcher m = new HistogramMatcher();
		int[] F = m.matchHistograms(hA, hB);
		
		for (int i=0; i<F.length; i++) {
			IJ.log(i + " -> " + F[i]);
		}
		
		ip.applyTable(F);
	}

	public boolean showDialog() {

		// get list of open images
		int[] windowList = WindowManager.getIDList();
		if(windowList==null){
			IJ.noImage();
			return false;
		}
		// get image titles
		String[] windowTitles = new String[windowList.length];
		for (int i = 0; i < windowList.length; i++) {
			ImagePlus imp = WindowManager.getImage(windowList[i]);
			if (imp != null)
				windowTitles[i] = imp.getShortTitle();
			else
				windowTitles[i] = "untitled";
		}


		// Create dialog and show
		GenericDialog gd = new GenericDialog("Methods");

        gd.addCheckbox("Histogram Equalization", runHistEqualization);
        gd.addCheckbox("Match Piecewise Linear Histogram", runPiecewise);
        gd.addCheckbox("Match to Gaussian Histogram",runGaussian);
        gd.addCheckbox("Match to Image Histogram",runMatchImage);
        gd.addChoice("Reference Image: ", windowTitles, windowTitles[0]);
        
        gd.showDialog();
        if (gd.wasCanceled())
            return false;

        runHistEqualization = gd.getNextBoolean();
        runPiecewise = gd.getNextBoolean();
        runGaussian = gd.getNextBoolean();
        runMatchImage = gd.getNextBoolean();
        int img2Index = gd.getNextChoiceIndex();
		imB = WindowManager.getImage(windowList[img2Index]);

        return true;
	}

	public boolean runDialog() {
		// get list of open images
		int[] windowList = WindowManager.getIDList();
		if(windowList==null){
			IJ.noImage();
			return false;
		}
		// get image titles
		String[] windowTitles = new String[windowList.length];
		for (int i = 0; i < windowList.length; i++) {
			ImagePlus imp = WindowManager.getImage(windowList[i]);
			if (imp != null)
				windowTitles[i] = imp.getShortTitle();
			else
				windowTitles[i] = "untitled";
		}
		// create dialog and show
		GenericDialog gd = new GenericDialog("Select Reference Image");
		gd.addChoice("Reference Image:", windowTitles, windowTitles[0]);
		gd.showDialog(); 
		if (gd.wasCanceled()) 
			return false;
		else {
			int img2Index = gd.getNextChoiceIndex();
			imB = WindowManager.getImage(windowList[img2Index]);
			return true;
		}
	}
	
}

