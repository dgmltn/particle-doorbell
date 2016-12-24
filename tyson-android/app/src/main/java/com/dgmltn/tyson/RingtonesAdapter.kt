package com.dgmltn.tyson

import android.content.Context
import android.support.v7.widget.RecyclerView
import android.support.v7.widget.RecyclerView.Adapter
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.RadioButton
import android.widget.TextView
import timber.log.Timber

class RingtonesAdapter(context: Context) : Adapter<RingtonesAdapter.ViewHolder>() {

    private val inflater: LayoutInflater
    private val items: List<RingtoneItem>

    var listener: ((RingtoneItem) -> (Unit))? = null

    init {
        this.inflater = LayoutInflater.from(context)
        this.items = RingtoneItem.generate(context)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        return ViewHolder(inflater.inflate(R.layout.row_ringtone, parent, false))
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.bind(position)
    }

    override fun getItemCount(): Int {
        return items.size
    }

    fun getItem(position: Int): RingtoneItem {
        return items[position]
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // ViewHolder
    ///////////////////////////////////////////////////////////////////////////////////////////////

    private var selectedIndex = 0

    inner class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {

        private val title by lazy { view.findViewById(R.id.title) as TextView }
        private val button by lazy { view.findViewById(R.id.button) as RadioButton }

        init {
            view.setOnClickListener {
                val position = adapterPosition
                if (position != selectedIndex) {
                    val oldIndex = selectedIndex
                    selectedIndex = position
                    notifyItemRangeChanged(oldIndex, 1)
                    notifyItemRangeChanged(selectedIndex, 1)
                    listener?.invoke(getItem(selectedIndex))
                }
            }
        }

        fun bind(position: Int) {
            title.text = getItem(position).entry
            button.isChecked = position == selectedIndex
        }
    }
}