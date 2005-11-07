//File: Dac.java

public class Dac {
	public native String dac_pl_cal(String case_sn,
				     String alias,
				     String uid,
				     String upw);

	static {

		System.loadLibrary("dacploan");
	}

	public static void main(String[] args) {
		String case_sn;
		String alias, uid, upw;
		String error_message;
		
		case_sn = "1";
		alias = "dacdb";
		uid = "ejcicap1";
		upw = "ejcicap1";
	
		Dac ploan = new Dac();
		error_message = ploan.dac_pl_cal(case_sn, alias, uid, upw);
		if (error_message.substring(0,1) != "0")
			System.out.println("Message: " + error_message);
	}
}


