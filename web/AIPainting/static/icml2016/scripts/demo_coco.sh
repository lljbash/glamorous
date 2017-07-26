
net_gen=coco_fast_t70_nc3_nt128_nz100_bs64_cls0.5_ngf196_ndf196_100_net_G.t7 \
net_txt=coco_gru18_bs64_cls0.5_ngf128_ndf128_a10_c512_80_net_T.t7 \
queries=scripts/coco_queries.txt \
dataset=coco \
luajit txt2img_demo.lua

