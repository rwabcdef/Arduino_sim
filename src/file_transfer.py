import os, shutil
import array

#embeddedRoot = r'C:\Users\rw123\OneDrive\Documents\Rob\software\Embedded\Arduino\Train\controller\v1\trn_cont_v1'
embeddedRoot = r'C:\Users\rw123\OneDrive\Documents\Rob\software\Embedded\Arduino\Train\controller\v1\trn_cont_v1b'

simulatedRoot = r'C:\Users\rw123\OneDrive\Documents\Rob\software\C++\eclipse\embed_sim\arduino\prj3\src'

# direction
#dirToEmbedded = False   # from embedded to simulated
dirToEmbedded = True   # from simulated to embedded

class FilePair(object):
  def __init__(self, embFile, simFile):
    self.embFile = os.path.join(embeddedRoot, embFile)
    self.simFile = os.path.join(simulatedRoot, simFile)

  def print(self):
    print('emb: ' + self.embFile)
    print('sim: ' + self.simFile)
    print('----------------------------------')

  def transfer(self, directionToEmbedded, debugPrint = False):
    if directionToEmbedded:
      src = self.simFile
      dst = self.embFile
    else:
      src = self.embFile
      dst = self.simFile
    shutil.copy2(src, dst)
    if debugPrint:
      print("%s -> %s" % (src, dst))

filePairs = [
             #FilePair('env_config.h', 'env\env_config.h'),
             FilePair('env.h', 'env\env.h'),
             FilePair('env.hpp', 'env\env.hpp'),
             FilePair('DebugPrint.hpp', 'DebugPrint\DebugPrint.hpp'),
             FilePair('DebugPrint.cpp', 'DebugPrint\DebugPrint.cpp'),
             FilePair('DebugUser.hpp', 'Arduino\Serlink\DebugUser\DebugUser.hpp'),
             FilePair('DebugUser.cpp', 'Arduino\Serlink\DebugUser\DebugUser.cpp'),
             FilePair('Frame.hpp', 'Arduino\Serlink\Transport\Frame\Frame.hpp'),
             FilePair('Frame.cpp', 'Arduino\Serlink\Transport\Frame\Frame.cpp'),
             FilePair('Reader_config.hpp', 'Arduino\Serlink\Transport\Reader\Reader_config.hpp'),
             FilePair('Reader.hpp', 'Arduino\Serlink\Transport\Reader\Reader.hpp'),
             FilePair('Reader.cpp', 'Arduino\Serlink\Transport\Reader\Reader.cpp'),
             FilePair('Writer_config.hpp', 'Arduino\Serlink\Transport\Writer\Writer_config.hpp'),
             FilePair('Writer.hpp', 'Arduino\Serlink\Transport\Writer\Writer.hpp'),
             FilePair('Writer.cpp', 'Arduino\Serlink\Transport\Writer\Writer.cpp'),
             FilePair('StateMachine.hpp', 'Arduino\StateMachine\StateMachine.hpp'),
             FilePair('StateMachine.cpp', 'Arduino\StateMachine\StateMachine.cpp'),
             FilePair('swTimer.h', 'Arduino\swTimer\swTimer.h'),
             FilePair('timer0.h', r'Arduino\hw\timer0\timer0.h'),
             FilePair('timer0.c', r'Arduino\hw\timer0\timer0.c'),
             FilePair(r'uart.h', r'Arduino\hw\uart\uart.h'),
             FilePair(r'uart.c', r'Arduino\hw\uart\uart.c'),
             FilePair(r'uart_wrapper.hpp', r'Arduino\cpp_wrappers\uart_wrapper.hpp'),
             FilePair(r'hw_gpio.h', r'Arduino\hw\gpio\hw_gpio.h'),
             FilePair(r'hw_gpio.c', r'Arduino\hw\gpio\hw_gpio.c'),
             FilePair('Socket.hpp', 'Arduino\Serlink\Transport\Socket\Socket.hpp'),
             FilePair('Socket.cpp', 'Arduino\Serlink\Transport\Socket\Socket.cpp'),
             FilePair('Transport_config.hpp', 'Arduino\Serlink\Transport\Transport_config.hpp'),
             FilePair('Transport.hpp', 'Arduino\Serlink\Transport\Transport.hpp'),
             FilePair('Transport.cpp', 'Arduino\Serlink\Transport\Transport.cpp'),
             FilePair('System0.hpp', 'System\System0\System0.hpp'),
             FilePair('System0.cpp', 'System\System0\System0.cpp')
             ]

toSimFilePairs = [
  FilePair('timer0.c', r'Arduino\hw\timer0\timer0.c'),
  #FilePair(r'uart.h', r'Arduino\hw\uart\uart.h'),
  FilePair(r'uart.c', r'Arduino\hw\uart\uart.c'),
  #FilePair(r'uart_wrapper.hpp', r'Arduino\cpp_wrappers\uart_wrapper.hpp'),
  #FilePair(r'hw_gpio.h', r'Arduino\hw\gpio\hw_gpio.h'),
  FilePair(r'hw_gpio.c', r'Arduino\hw\gpio\hw_gpio.c')
]

def toEmbedded():
  for fp in filePairs:
    #fp.print()
    fp.transfer(dirToEmbedded, True)

def toSim():
  for fp in toSimFilePairs:
    fp.print()
    fp.transfer(False, False)

if __name__ == '__main__':

  print('main start 1')

  toEmbedded()
  #toSim()

  print('Main end')
