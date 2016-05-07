import sys
import os
import shlex
import subprocess
read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'
if read_the_docs_build:
    subprocess.call('doxygen', shell=True)
extensions = ['breathe']
breathe_projects = { 'firebase-arduino': 'xml' }
breathe_default_project = "firebase-arduino"
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'
project = u'firebase-arduino'
copyright = u'2015, firebase-arduino'
author = u'firebase-arduino'
version = '1.0'
release = '1.0'
language = None
exclude_patterns = ['_build']
pygments_style = 'sphinx'
todo_include_todos = False
html_static_path = ['_static']
htmlhelp_basename = 'firebase-arduinodoc'
latex_elements = {
}
latex_documents = [
  (master_doc, 'firebase-arduino.tex', u'firebase-arduino Documentation',
   u'firebase-arduino', 'manual'),
]
