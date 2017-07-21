from django.http import HttpResponse
from django.http import HttpResponseRedirect
from django.shortcuts import render
from django.shortcuts import render_to_response
from django.views.decorators.csrf import csrf_exempt
from PIL import Image

from ctypes import byref, cdll, c_int
import ctypes
lualib = ctypes.CDLL("/home/lljbash/torch/install/lib/libluajit.so", mode=ctypes.RTLD_GLOBAL)
import app

import urllib
# import urllib2

global content
global style

def homepage(request):
    
    global content
    global style
    myapp = app.GlamorousApp()
    myapp.initialize('/home/lljbash/data')
    content = request.GET.get("content")
    style = request.GET.get("style")

    if(content == None):
        return render_to_response("AIPainting.html")
    else:
        
        
        filename = myapp.transfer(content, int(style))


        return HttpResponseRedirect("/consequence?filename=%s"%filename)

def consequence(request):
    stylelist = ['Abstract painting','Post-impression','Neo-impression','Chinese ink painting','Suprematism','Impressionism']
    order = int(style) - 1

    #width = 708
    #height = 708
    filename = request.GET.get('filename')
    img = Image.open(filename)
    imgSize = img.size
    print(imgSize)
    
    return render_to_response("Paintcons.html",{'content':content,'style':stylelist[order],'image':'static/'+filename})
