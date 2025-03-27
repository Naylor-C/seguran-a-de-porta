// No seu Activity
class MainActivity : AppCompatActivity() {
    private var bluetoothSocket: BluetoothSocket? = null
    private lateinit var outputStream: OutputStream
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        
        // Botões
        btnAbrir.setOnClickListener { sendCommand('A') }
        btnFechar.setOnClickListener { sendCommand('F') }
        
        // Inicializa Bluetooth
        val adapter = BluetoothAdapter.getDefaultAdapter()
        val device = adapter.getRemoteDevice("00:12:6F:XX:XX:XX") // MAC do HC-05
        val uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB") // UUID padrão SPP
        
        try {
            bluetoothSocket = device.createRfcommSocketToServiceRecord(uuid)
            bluetoothSocket?.connect()
            outputStream = bluetoothSocket?.outputStream!!
        } catch (e: IOException) {
            e.printStackTrace()
        }
    }
    
    private fun sendCommand(cmd: Char) {
        try {
            outputStream.write(cmd.toInt())
        } catch (e: IOException) {
            e.printStackTrace()
        }
    }
    
    override fun onDestroy() {
        super.onDestroy()
        bluetoothSocket?.close()
    }
}
