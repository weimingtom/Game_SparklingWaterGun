inline float4 mulScreen(float4 Pos, float4x4 mat)
{
	float4 Tex;
	//テクスチャ座標計算
	float4 p;
	p = Pos;
	p = mul(p, mat);
	p.xyz /= p.w;
	//スクリーン変換
	p.y = -p.y;
	p.xy = 0.5f * p.xy + 0.5f;
	Tex = p;
	return Tex;
}

inline float GetCSM(float3 Tex, sampler samp)
{
	//シャドウマップから深度を取り出す
	float d = tex2D(samp, Tex.xy).r;
	//ここに例外処理を入れる
	if (d == 1) return 1;
	float l = (d < Tex.z + AdjustValue) ? ShadowLevel : 1;	//判定をぬるめにする
	return l;
}

inline float GetCVSM(float3 Tex, sampler samp)
{
	//シャドウマップから深度を取り出す
	float depth = tex2D(samp, Tex.xy).r;	//平均値E(x)
	float sqdepth = tex2D(samp, Tex.xy).g; //E(x^2)
	//分散値σ^2
	float variance = sqdepth - (depth * depth);

	//ここからチェビチェフの不等式の計算
	float md = Tex.z - depth;

	float rate = variance / (variance + (md * md));

	return saturate(max(rate, depth >= Tex.z));
}

//シグモイド
float	sigmoid(float gain,float x,float adjust)
{
	//
	return 1.0 /(1.0 + exp(-gain * (x - adjust)));

}