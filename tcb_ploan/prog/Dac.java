//File: Dac.java

public class Dac {
	public native String dac_pl_cal(String case_sn,
				     String alias,
				     String uid,
				     String upw);

	static {

		System.loadLibrary("dac");
	}

	public static void main(String[] args) {
		String case_sn;
		String alias, uid, upw;
		String error_message;
		
		case_sn = "002000008587";
		alias = "escore";
		uid = "escorap1";
		upw = "escorap1";
	
		Dac ploan = new Dac();
		error_message = ploan.dac_pl_cal(case_sn, alias, uid, upw);
		if (error_message.substring(0,4) != "0000")
			System.out.println("Message: " + error_message);
	}
}


