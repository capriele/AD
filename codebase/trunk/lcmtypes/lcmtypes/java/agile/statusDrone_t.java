/* LCM type definition class file
 * This file was automatically generated by lcm-gen
 * DO NOT MODIFY BY HAND!!!!
 */

package agile;
 
import java.io.*;
import java.util.*;
import lcm.lcm.*;
 
public final class statusDrone_t implements lcm.lcm.LCMEncodable
{
    public long timestampJetson;
    public short status;
 
    public statusDrone_t()
    {
    }
 
    public static final long LCM_FINGERPRINT;
    public static final long LCM_FINGERPRINT_BASE = 0xca2a7aa8b75afce9L;
 
    static {
        LCM_FINGERPRINT = _hashRecursive(new ArrayList<Class<?>>());
    }
 
    public static long _hashRecursive(ArrayList<Class<?>> classes)
    {
        if (classes.contains(agile.statusDrone_t.class))
            return 0L;
 
        classes.add(agile.statusDrone_t.class);
        long hash = LCM_FINGERPRINT_BASE
            ;
        classes.remove(classes.size() - 1);
        return (hash<<1) + ((hash>>63)&1);
    }
 
    public void encode(DataOutput outs) throws IOException
    {
        outs.writeLong(LCM_FINGERPRINT);
        _encodeRecursive(outs);
    }
 
    public void _encodeRecursive(DataOutput outs) throws IOException
    {
        outs.writeLong(this.timestampJetson); 
 
        outs.writeShort(this.status); 
 
    }
 
    public statusDrone_t(byte[] data) throws IOException
    {
        this(new LCMDataInputStream(data));
    }
 
    public statusDrone_t(DataInput ins) throws IOException
    {
        if (ins.readLong() != LCM_FINGERPRINT)
            throw new IOException("LCM Decode error: bad fingerprint");
 
        _decodeRecursive(ins);
    }
 
    public static agile.statusDrone_t _decodeRecursiveFactory(DataInput ins) throws IOException
    {
        agile.statusDrone_t o = new agile.statusDrone_t();
        o._decodeRecursive(ins);
        return o;
    }
 
    public void _decodeRecursive(DataInput ins) throws IOException
    {
        this.timestampJetson = ins.readLong();
 
        this.status = ins.readShort();
 
    }
 
    public agile.statusDrone_t copy()
    {
        agile.statusDrone_t outobj = new agile.statusDrone_t();
        outobj.timestampJetson = this.timestampJetson;
 
        outobj.status = this.status;
 
        return outobj;
    }
 
}
