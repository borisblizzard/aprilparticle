package com.aprilparticle.demoParticle;

public class DemoParticle extends com.april.Activity
{
	static
	{
		System.loadLibrary("demo_particle");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(com.april.NativeInterface.apkPath); // forces APK as archive file
	}
	
}
